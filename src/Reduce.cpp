/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 **/

#include "headers/Reduce.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "headers/FileManager.h"

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::regex;
using std::regex_search;
using std::smatch;
using std::sregex_iterator;
using std::string;
using std::to_string;
using std::vector;

/**
 * Class Constructor specifying output directory
 */
Reduce::Reduce(string input_file_path, string output_dir) {
  inputFilePath = input_file_path;
  outputDir = output_dir;
  FileManager fileManager;
}

void Reduce::reduce(string key, vector<int> intIterator) {
  // Sum all the values in the list
  int sum = 0;
  for (auto i = intIterator.begin(); i != intIterator.end(); i++) {
    sum += *i;
  }

  exportResult(key, sum);
}

void Reduce::exportResult(string key, int value) {
  string content = "(" + key + "," + to_string(value) + ")" + "\n";

  string fileName = "output.txt";

  bool isSuccessfulWrite =
    fileManager.writeFile(FileManager::APPEND, outputDir, fileName, content);
}

bool Reduce::processSortResult() {

  smatch match;
  regex r(R"((\"\w+\"),\s*\[(\d+(,\s*\d+)*)\])");

  array<string, 2> inputFile = fileManager.readFile(inputFilePath);
  string line = inputFile[1];

  while (regex_search(line, match, r)) {
    string word = match[1].str();
    string ones = match[2].str();

    vector<int> onesList;
    regex one_r("\\d+");

    for (sregex_iterator it(ones.begin(), ones.end(), one_r), end_it;
          it != end_it; ++it) {
      int num = stoi(it->str());
      onesList.push_back(num);
    }

    reduce(word, onesList);
    line = match.suffix();
  }

  writeSuccess();
  return true;
}

void Reduce::writeSuccess() {

  bool isSuccessfulWrite =
      fileManager.writeFile(FileManager::CREATE, outputDir, "SUCCESS.txt", "SUCCESS");
}
