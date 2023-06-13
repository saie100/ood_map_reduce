#include "headers/Workflow.h"

#include <string>
#include <vector>
#include <thread>
#ifdef _WIN32
#include <windows.h>
#else
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

  controller.listenTo(controller_port, procNum);

  Socket stub1("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  //Socket stub2("stub", mapDllPath, reduceDllPath, inputReduceDir, tempDir, tempMapOutputFilePath);
  stub1.listenTo(8080, 1);
  //stub2.listenTo(8082, 1);
  

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

  controller.connectTo(8080);

  controller.sendMessage("start mapper:0,1,2,3,4,5,6,7,8,9", 8080); // port 8080
  //std::this_thread::sleep_for(std::chrono::milliseconds(7000));
  //controller.waitForThreads();
  
  controller.sendMessage("start reducer:0,1,2,3,4,5,6,7,8,9", 8080); // port 8080
  std::this_thread::sleep_for(std::chrono::milliseconds(7000));
  controller.sendMessage("do not do anything please", 8080); // port 8080
  


  string reduceTempDir = tempDir + "/reduce";

  cout << "Sorting and aggregating complete!\n" << "Aggregating sorted output..." << endl;

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
