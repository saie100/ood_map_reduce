#include "headers/Workflow.h"

#include <string>
#include <vector>

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
Workflow::Workflow(string input_dir, string temp_dir, string output_dir)
    : inputDir(input_dir), tempDir(temp_dir), outputDir(output_dir) {}

void Workflow::start() {
  FileManager fm = FileManager();
  vector<string> inputFilePaths = fm.getFilesFromDir(inputDir);

  string tempMapOutputFilePath = tempDir + "/tempMapOutput.txt";
  string tempSortOutputFilePath = tempDir + "/tempSortOutput.txt";

  Map m = Map(tempMapOutputFilePath);
  Sort s = Sort(tempMapOutputFilePath, tempSortOutputFilePath);
  Reduce r = Reduce(tempSortOutputFilePath, outputDir);

  for (string inputFilePath : inputFilePaths) {
    array<string, 2> inputFile = fm.readFile(inputFilePath);
    string inputFileName = inputFile[0];
    string inputContent = inputFile[1];

    m.map(inputFileName, inputContent);
  }

  s.Sorter();

  r.processSortResult();
}
