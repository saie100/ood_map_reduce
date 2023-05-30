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
#endif

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir), reduceDllPath(reduce_dll_path), mapDllPath(map_dll_path) {}

mutex mapMutex;

void mapProcess(int threadId, int numInputFiles, string mapDllPath, string inputDir, string outputFilePath) {

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
    //TODO delete the file once we have mapped it
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

void Workflow::start() {

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

  FileManager fm = FileManager();
  fm.deleteFilesFromDir(tempDir);
  fm.deleteFilesFromDir(outputDir);
  vector<string> inputFilePaths = fm.getFilesFromDir(inputDir);

  string tempMapOutputFilePath = tempDir + "/tempMapOutput.txt";
  string tempSortOutputFilePath = tempDir + "/tempSortOutput.txt";

  Sort s = Sort(tempMapOutputFilePath, tempSortOutputFilePath);

  int numProcesses = 10;

  cout << "Mapping input files..." << endl;
  for (string inputFilePath : inputFilePaths) {
    vector<array<string, 2>> inputFile = fm.partitionFile(inputFilePath, numProcesses);
    for (array<string, 2> partition : inputFile) {
        string inputFileName = partition[0];
        string path = tempDir + "/" + inputFileName;
        string inputContent = partition[1];
        // write the data to the file
        FileManager::writeFile(FileManager::MODE::APPEND, path, inputContent);
    }
  }

  thread mapThreads[numProcesses];
  // Start each thread
    for (int i = 0; i < numProcesses; ++i) {
        mapThreads[i] = thread(mapProcess, i, inputFilePaths.size(), mapDllPath, tempDir, tempMapOutputFilePath);
    }
    
    // Wait for each thread to finish
    for (int i = 0; i < numProcesses; ++i) {
        mapThreads[i].join();
    }
//   cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;

//   s.Sorter();
//   cout << "Sorting and aggregating complete!\n" << "Reducing output..." << endl;

// #ifdef _WIN32
//     // use reduce dll to reduce
//     if (reduceDLL != NULL) {
//       funcReduce processSortResult = (funcReduce)GetProcAddress(reduceDLL, "processSortResult");
//       if (processSortResult != NULL) {
//         processSortResult(tempSortOutputFilePath, outputDir);
//       } else {
//         cout << "Reduce DLL not found" << endl;
//         exit(1);
//       }
//     }
// #else
//   processSortResult(tempSortOutputFilePath, outputDir);
// #endif
//   cout << "Reduce complete!" << endl;
}
