#include "headers/Workflow.h"

#include <string>
#include <vector>
#include <dlfcn.h>
#include <iostream>

#include "headers/FileManager.h"
#include "headers/Map.hpp"
#include "headers/Reduce.h"
#include "headers/Sort.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir), reduceDllPath(reduce_dll_path), mapDllPath(map_dll_path) {}

void Workflow::start() {

#ifdef _WIN32
  // todo
#else
  void* ReducelibraryHandle = dlopen(reduceDllPath.c_str(), RTLD_LAZY);
  // void* MaplibraryHandle = dlopen(mapDllPath.c_str(), RTLD_LAZY);

  if (!ReducelibraryHandle) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }

  typedef void (*ProcessSortResult)(const string, const string);
  // typedef void (*Map)(const string&, const string&, const string&);

  ProcessSortResult processSortResult = (ProcessSortResult) dlsym(ReducelibraryHandle, "processSortResult");
  // Map map = (Map) dlsym(MaplibraryHandle, "map");

  if (!processSortResult) {
    printf("Error: %s\n", dlerror());
    exit(1);
  }
#endif

  FileManager fm = FileManager();
  vector<string> inputFilePaths = fm.getFilesFromDir(inputDir);

  string tempMapOutputFilePath = tempDir + "/tempMapOutput.txt";
  string tempSortOutputFilePath = tempDir + "/tempSortOutput.txt";

  Map m = Map(tempMapOutputFilePath);
  Sort s = Sort(tempMapOutputFilePath, tempSortOutputFilePath);

  cout << "Mapping input files..." << endl;
  for (string inputFilePath : inputFilePaths) {
    array<string, 2> inputFile = fm.readFile(inputFilePath);
    string inputFileName = inputFile[0];
    string inputContent = inputFile[1];

    // map(inputFileName, inputContent, tempMapOutputFilePath);
    m.map(inputFileName, inputContent);
  }
  cout << "Mapping complete!\n" << "Sorting and aggregating map output..." << endl;

  s.Sorter();
  cout << "Sorting and aggregating complete!\n" << "Reducing output..." << endl;

  processSortResult(tempSortOutputFilePath, outputDir);
  cout << "Reduce complete!" << endl;
}
