#include "headers/Workflow.h"

#include <string>
#include <thread>
#ifndef _WIN32
#include <dlfcn.h>
#endif

#include "headers/FileManager.h"
#include "headers/Sort.h"
#include "headers/Socket.h"


using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::to_string;

#ifdef _WIN32
typedef void (*Aggregate)(const string, const string);
#endif

// define the static stubs vector
vector<Workflow::Stub> Workflow::stubs;
mutex Workflow::stub_locker;

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path, int proc_num)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir), reduceDllPath(reduce_dll_path), mapDllPath(map_dll_path), procNum(proc_num) {}

void Workflow::start() {

  #ifdef _WIN32
    // create DLL handles
    HINSTANCE reduceDLL = LoadLibraryA(reduceDllPath.c_str());
  #else
    void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);
    typedef void (*Aggregate)(const string, const string);
    Aggregate aggregate = (Aggregate) dlsym(ReducelibraryHandle, "aggregate");

    if (!aggregate) {
        printf("Error: %s\n", dlerror());
        exit(1);
    }
  #endif

  FileManager fm = FileManager();
  fm.deleteFilesFromDir(tempDir);
  fm.deleteFilesFromDir(outputDir);
  
  vector<string> inputFilePaths = fm.getFilesFromDir(inputDir);

  string tempMapOutputDir = tempDir + "/map";
  fm.createDir(tempMapOutputDir);
  string tempMapOutputFilePath = tempMapOutputDir + "/tempMapOutput.txt";
  string tempSortOutputFilePath = tempDir + "/tempSortOutput.txt";
  string partitionsDir = tempDir + "/partitions";
  string inputReduceDir = tempDir + "/map";
  fm.createDir(partitionsDir);
  fm.createDir(tempDir + "/sort");
  fm.createDir(tempDir + "/reduce");

  Socket controller("controller", "", "", "", "", "");

  controller.listenTo(controller_port, (procNum*2));

  // set up stub info
  int portNum = 8090;
  int maxStubCount = 5;
  int stubCount = 0;
  stub_locker.lock();
  // create stubs
  Socket stubSocket1("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  Socket stubSocket2("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  Socket stubSocket3("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  Socket stubSocket4("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  Socket stubSocket5("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  // loop through the stubs and set up all the info in the Stub vector
  for (int i = 0; i < procNum; i++) {
    if (stubCount < maxStubCount) {
      if (stubCount == 0) {
        stubSocket1.listenTo(portNum, 1);
      } else if (stubCount == 1) {
        stubSocket2.listenTo(portNum, 1);
      } else if (stubCount == 2) {
        stubSocket3.listenTo(portNum, 1);
      } else if (stubCount == 3) {
        stubSocket4.listenTo(portNum, 1);
      } else if (stubCount == 4) {
        stubSocket5.listenTo(portNum, 1);
      }
      // create tracking Stub
      Stub stub;
      stub.port_num = portNum;
      portNum++;
      // create thread data
      ThreadStatus thread;
      thread.threadId = i;
      thread.done = false;
      // add the thread to the vector in Stub
      stub.threads.emplace_back(thread);
      // add the stub to the stub vector
      stubs.emplace_back(stub);
      stubCount++;
    } else {
      // the stub is already created, so just add the thread info to its vector
      ThreadStatus thread;
      thread.threadId = i;
      thread.done = false;
      stubs[i % maxStubCount].threads.emplace_back(thread);
    }
  }
  stub_locker.unlock();

  cout << "Mapping input files..." << endl;
  for (string inputFilePath : inputFilePaths) {
    vector<array<string, 2>> inputFile = fm.partitionFile(inputFilePath, procNum);
    for (array<string, 2> partition : inputFile) {
        string inputFileName = partition[0];
        string path = partitionsDir + "/" + inputFileName;
        string inputContent = partition[1];
        // write the data to the file
        FileManager::writeFile(FileManager::MODE::APPEND, path, inputContent);
    }
  }

  stub_locker.lock();
  // connect the controller to all the stubs
  for (Stub stub : stubs) {
    controller.connectTo(stub.port_num);
  }

  // send the mapper messages to the stubs
  for (Stub stub : stubs) {
    string message = "start mapper:";
    message += to_string(stub.threads[0].threadId);
    for (int i = 1; i < stub.threads.size(); i++) {
      message += ",";
      message += to_string(stub.threads[i].threadId);
    }
    controller.sendMessage(message, stub.port_num);
  }
  stub_locker.unlock();

  while (!stubsAreDone()) {
    // empty loop until the stubs are all done
  }
  cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;
  // reset all the stubs for the reduce processes
  resetStubs();
  // send the reducer messages to the stubs
  for (Stub stub : stubs) {
    string message = "start reducer:";
    message += to_string(stub.threads[0].threadId);
    for (int i = 1; i < stub.threads.size(); i++) {
      message += ",";
      message += to_string(stub.threads[i].threadId);
    }
    controller.sendMessage(message, stub.port_num);
  }

  while (!stubsAreDone()) {
    // empty loop until the stubs are all done
  }

  // kill all the listening threads as communication is now complete
  Socket::setStopListening();
  cout << "Sorting and aggregating complete!\n" << "Aggregating sorted output..." << endl;

  string reduceTempDir = tempDir + "/reduce";

#ifdef _WIN32
// use reduce dll to sort and reduce
if (reduceDLL != NULL) {
    Aggregate aggregate = (Aggregate)GetProcAddress(reduceDLL, "aggregate");
    if (aggregate != NULL) {
        aggregate(reduceTempDir, outputDir);
    } else {
        cout << "Aggregate DLL not found" << endl;
        exit(1);
    }
}
#else
  aggregate(reduceTempDir, outputDir);
#endif

  cout << "Aggregating complete!" << endl;

}

void Workflow::resetStubs() {
  stub_locker.lock();
  for (Stub &stub : stubs) {
    for (ThreadStatus &thread : stub.threads) {
      thread.done = false;
    }
  }
  stub_locker.unlock();
}

void Workflow::setStubDone(int threadId, bool done) {
  stub_locker.lock();
  for (Stub &stub : stubs) {
    for (ThreadStatus &thread : stub.threads) {
      if (thread.threadId == threadId) {
        thread.done = done;
      }
    }
  }
  stub_locker.unlock();
}

bool Workflow::stubsAreDone() {
  bool allDone = true;
  stub_locker.lock();
  for (Stub stub : stubs) {
    for (ThreadStatus thread : stub.threads) {
      if (!thread.done) {
        allDone = false;
        stub_locker.unlock();
        return allDone;
      }
    }
  }
  stub_locker.unlock();
  return allDone;
}
