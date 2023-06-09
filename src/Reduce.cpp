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

  // Write each result to the final output file in the format of
  // ("word", integer)
  string content = "(" + key + "," + to_string(value) + ")" + "\n";

  string fileName = "/output.txt";

  bool isSuccessfulWrite =
    FileManager::writeFile(FileManager::APPEND, outputDir, fileName, content);
}

bool Reduce::processSortResult() {

  // Parse the intermediate file produced by the sort class
  array<string, 2> inputFile = fileManager.readFile(inputFilePath);
  string line = inputFile[1];

  // Find the first left parenthese
  size_t leftParen = line.find("(");

  // find the first right parenthese
  while (leftParen != string::npos){
    size_t rightParen = line.find(")", leftParen+1);
    if (rightParen == string::npos){
      break;
    }

    // extract the data that's between the two parenthese
    string token = line.substr(leftParen+1, rightParen-1);

    // Find the comma that separates word and integer array
    size_t commaPos = token.find(",");
    if (commaPos == string::npos) {
      break;
    }

    // get the word out of the toekn
    string word = token.substr(0, commaPos);
    size_t squareBracketLeft = token.find("[");
    size_t squareBracketRight = token.find("]");

    if (squareBracketLeft == string::npos || squareBracketRight == string::npos) {
      break;
    }

    // Get the ones out of the token
    string numbers = token.substr(squareBracketLeft+1, squareBracketRight-squareBracketLeft-1);
    vector<int> onesList;
    size_t startPos = 0;

    while (startPos < numbers.size()) {
      size_t commaPos = numbers.find(",", startPos);
      if (commaPos == string::npos) {
          commaPos = numbers.size();
      }
      string int_str = numbers.substr(startPos, commaPos-startPos);
      onesList.push_back(stoi(int_str));
      startPos = commaPos + 1;
    }

    reduce(word, onesList);
    leftParen = line.find("(", rightParen+1);

  }

  // Write success file once finished parsing input
  writeSuccess();
  return true;
}

void Reduce::writeSuccess() {

  bool isSuccessfulWrite =
      fileManager.writeFile(FileManager::CREATE, outputDir, "/SUCCESS.txt", "");
}
