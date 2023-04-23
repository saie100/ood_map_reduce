/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 **/

#include "Reduce.h"
#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::to_string;
using std::ifstream;
using std::cerr;
using std::endl;
using std::getline;
using std::smatch;
using std::regex_search;
using std::regex;
using std::sregex_iterator;


/**
 * Class Constructor specifying output directory
 */
Reduce::Reduce(string input_file_path, string output_dir){
  inputFilePath = input_file_path;
  outputDir = output_dir;
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

  string content = key + " : " + to_string(value);
  FileManager fm = FileManager();

  // if we are going with single output file
  string fileName = "output.txt";

  bool isSuccessfulWrite = fm.writeFile(
    FileManager::APPEND, outputDir, fileName, content
  );

}

bool Reduce::processSortResult() {
  // read the intermediate file
  ifstream file(inputFilePath);

  if (!file){
    cerr << "Error: could not open file" << endl;
    return false;
  }

  string line;
  smatch match;
  regex r(R"((\"\w+\"),\s*\[(\d+(,\s*\d+)*)\])");

  while (getline(file, line)){
    while(regex_search(line, match, r)) {

      string word = match[1].str();
      string ones = match[2].str();

      vector< int > onesList;
      regex one_r("\\d+");

      for (sregex_iterator it(ones.begin(), ones.end(), one_r), end_it; it != end_it; ++it) {
          int num = stoi(it->str());
          onesList.push_back(num);
      }

      reduce(word, onesList);
      line = match.suffix();
    }
  }

  writeSuccess();
  return true;
}

void Reduce::writeSuccess(){
  FileManager fm = FileManager();
  bool isSuccessfulWrite = fm.writeFile(
    FileManager::CREATE, outputDir, "SUCCESS.txt", "SUCCESS"
  );
}
