#include "headers/Workflow.h"

#include <string>
#include <vector>
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

#ifdef _WIN32
typedef void (*funcMap)(const string&, const string&, const string&);
typedef void (*funcReduce)(string, string);
#endif

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir), reduceDllPath(reduce_dll_path), mapDllPath(map_dll_path) {}

void Workflow::start() {

#ifdef _WIN32
  // create DLL handles
  HINSTANCE mapDLL = LoadLibraryA(mapDllPath.c_str());
  HINSTANCE reduceDLL = LoadLibraryA(reduceDllPath.c_str());
#else
  void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);
  void* MaplibraryHandle = dlopen(mapDllPath.c_str(), RTLD_LAZY);

  if (!ReducelibraryHandle) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }

  typedef void (*ProcessSortResult)(const string, const string);
  typedef void (*Map)(const string&, const string&, const string&);

  ProcessSortResult processSortResult = (ProcessSortResult) dlsym(ReducelibraryHandle, "processSortResult");
  Map map = (Map) dlsym(MaplibraryHandle, "map");

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

  cout << "Mapping input files..." << endl;
  for (string inputFilePath : inputFilePaths) {
    array<string, 2> inputFile = fm.readFile(inputFilePath);
    string inputFileName = inputFile[0];
    string inputContent = inputFile[1];

#ifdef _WIN32
    // use map dll to map
    if (mapDLL != NULL) {
      funcMap map = (funcMap)GetProcAddress(mapDLL, "map");
      if (map != NULL) {
        map(inputFileName, inputContent, tempMapOutputFilePath);
      } else {
        cout << "Map DLL not found" << endl;
        exit(1);
      }
    }
#else
    map(inputFileName, inputContent, tempMapOutputFilePath);
#endif
  }
  cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;

  s.Sorter();
  cout << "Sorting and aggregating complete!\n" << "Reducing output..." << endl;

#ifdef _WIN32
    // use reduce dll to reduce
    if (reduceDLL != NULL) {
      funcReduce processSortResult = (funcReduce)GetProcAddress(reduceDLL, "processSortResult");
      if (processSortResult != NULL) {
        processSortResult(tempSortOutputFilePath, outputDir);
      } else {
        cout << "Reduce DLL not found" << endl;
        exit(1);
      }
    }
#else
  processSortResult(tempSortOutputFilePath, outputDir);
#endif
  cout << "Reduce complete!" << endl;
}
