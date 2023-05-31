#include "headers/Workflow.h"

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "headers/FileManager.h"
#include "headers/Sort.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::to_string;
using std::mutex;

#ifdef _WIN32
typedef void (*funcMap)(const string&, const string&, const string&);
typedef void (*funcReduce)(string, string);
typedef void (*Aggregate)(const string, const string);
#endif

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path, int proc_num)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir), reduceDllPath(reduce_dll_path), mapDllPath(map_dll_path), procNum(proc_num) {}

mutex mapMutex;


void mapProcess(int threadId, string mapDllPath, string inputDir, string outputFilePath) {

#ifdef _WIN32
  // create DLL handles
  HINSTANCE mapDLL = LoadLibraryA(mapDllPath.c_str());
#else
  void* MaplibraryHandle = dlopen(mapDllPath.c_str(), RTLD_LAZY);

  if (!MaplibraryHandle) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }

  typedef void (*Map)(const string&, const string&, const string&);

  Map map = (Map) dlsym(MaplibraryHandle, "map");

  if (!map) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }
#endif

  vector<string> inputFilePaths = FileManager::getFilesFromDir(inputDir);
  int filesMapped = 0;
  for (string inputFilePath : inputFilePaths) {
    auto fileName = FileManager::getFilename(inputFilePath);
    auto firstUnderscore = fileName.find("_");
    auto secondUnderscore = fileName.find("_", firstUnderscore + 1);
    auto threadNum = fileName.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
    if (threadNum == to_string(threadId)) {
      auto inputFile = FileManager::readFile(inputFilePath);
      auto threadOutputPath = outputFilePath;
      size_t period = threadOutputPath.find(".");
      threadOutputPath = threadOutputPath.substr(0, period);
      threadOutputPath = threadOutputPath + "_" + to_string(threadId) + "_" + ".txt";
      auto inputFileName = inputFile[0];
      auto inputContent = inputFile[1];
#ifdef _WIN32
      // use map dll to map
      if (mapDLL != NULL) {
        funcMap map = (funcMap)GetProcAddress(mapDLL, "map");
        if (map != NULL) {
          map(inputFileName, inputContent, threadOutputPath);
        } else {
          cout << "Map DLL not found" << endl;
          exit(1);
        }
      }
#else
      map(inputFileName, inputContent, threadOutputPath);
#endif
    }
  }
}


void reduceProcess(int threadId, string reduceDllPath, string inputDir, string tempDir) {

  #ifdef _WIN32
    // create DLL handles
    HINSTANCE reduceDLL = LoadLibraryA(reduceDllPath.c_str());
  #else
    void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);

    if (!ReducelibraryHandle) {
      printf("Error: %s\n", dlerror());
      exit(1);
    }

    typedef void (*ProcessSortResult)(const string, const string);

    ProcessSortResult processSortResult = (ProcessSortResult) dlsym(ReducelibraryHandle, "processSortResult");

    if (!processSortResult) {
      printf("Error: %s\n", dlerror());
      exit(1);
    }
  #endif

  vector<string> inputFilePaths = FileManager::getFilesFromDir(inputDir);
  int filesReduced = 0;
  for (string inputFilePath : inputFilePaths) {
  
    //TODO delete the file once we have mapped it
    auto fileName = FileManager::getFilename(inputFilePath);
    auto firstUnderscore = fileName.find("_");
    auto secondUnderscore = fileName.find("_", firstUnderscore + 1);
    auto threadNum = fileName.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);

    if (threadNum == to_string(threadId)) {
      auto inputFile = FileManager::readFile(inputFilePath);
      auto inputFileName = inputFile[0];
      auto inputContent = inputFile[1];

    #ifdef _WIN32
      // use reduce dll to sort and reduce
      if (reduceDLL != NULL) {
        funcReduce processSortResult = (funcReduce)GetProcAddress(reduceDLL, "processSortResult");
        if (processSortResult != NULL) {
          processSortResult(inputFilePath, tempDir);
        } else {
          cout << "Reduce DLL not found" << endl;
          exit(1);
        }
      }
    #else
      processSortResult(inputFilePath, tempDir);
    #endif
    }
  }
}

void Workflow::start() {

  #ifdef _WIN32
    // create DLL handles
    HINSTANCE reduceDLL = LoadLibraryA(reduceDllPath.c_str());
  #else
    void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);

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
  fm.createDir(partitionsDir);
  cout << "Mapping input files..." << endl;
  for (string inputFilePath : inputFilePaths) {
    vector<array<string, 2>> inputFile = fm.partitionFile(inputFilePath, procNum);
    for (array<string, 2> partition : inputFile) {
        string inputFileName = partition[0];
        fm.createDir(partitionsDir);
        string path = partitionsDir + "/" + inputFileName;
        string inputContent = partition[1];
        // write the data to the file
        FileManager::writeFile(FileManager::MODE::APPEND, path, inputContent);
    }
  }

  std::vector<std::thread> mapThreads(procNum);
  // Start each thread
  for (int i = 0; i < procNum; ++i) {
      mapThreads[i] = thread(mapProcess, i, mapDllPath, tempDir, tempMapOutputFilePath);
  }
  
  // Wait for each thread to finish
  for (int i = 0; i < procNum; ++i) {
      mapThreads[i].join();
  }
  cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;

  thread reduceThreads[procNum];
  // Start each thread
  for (int i = 0; i < procNum; ++i) {
    reduceThreads[i] = thread(reduceProcess, i, reduceDllPath, tempDir + "/map", tempDir);
  }
  // Wait for each thread to finish
  for (int i = 0; i < procNum; ++i) {
    reduceThreads[i].join();
  }
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

}
