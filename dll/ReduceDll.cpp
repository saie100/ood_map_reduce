#include "../src/headers/FileManager.h"
#include "../src/headers/Sort.h"

#include <vector>
#include <iostream>
#include <random>
#include <map>
#include <thread>
#include <chrono>
#include <sstream>

using std::map;
using std::stoi;
using std::to_string;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::vector;
using std::sort;
using std::pair;


// Helper function to compare map elements by value
bool compareByValue(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

void exportResult(string key, int value, string outputDir, string fileName) {

  // Write each result to the final output file in the format of
  // ("word", integer)
  string content = "(" + key + "," + to_string(value) + ")" + "\n";

  bool isSuccessfulWrite =
    FileManager::writeFile(FileManager::APPEND, outputDir, fileName, content);
}

void exportResult(map<string, int> result, string outputDir){
  // sort the final result
  vector<pair<string, int>> sortedVec(result.begin(), result.end());
  sort(sortedVec.begin(), sortedVec.end(), compareByValue);
  for (const auto& entry: sortedVec){
    exportResult(entry.first, entry.second, outputDir, "/output.txt");
  }
}

void reduce(string key, vector<int> intIterator, string outputDir, string fileName) {
  // Sum all the values in the list
  int sum = 0;
  for (auto i = intIterator.begin(); i != intIterator.end(); i++) {
    sum += *i;
  }
  exportResult(key, sum, outputDir, fileName);
}

void writeSuccess(FileManager fileManager, string outputDir) {

  bool isSuccessfulWrite =
      fileManager.writeFile(FileManager::CREATE, outputDir, "/SUCCESS.txt", "");
}

extern "C" void aggregate(string tempDir, string outputDir) {

  FileManager fileManager;
  vector<string> reduceResultFiles = fileManager.getFilesFromDir(tempDir);

  map<string, int> result;

  for(string reduceResultFile: reduceResultFiles){
      array<string, 2> inputFile = fileManager.readFile(reduceResultFile);
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
        string token = line.substr(leftParen+1, rightParen-leftParen-1);

        // Find the comma that separates word and integer
        size_t commaPos = token.find(",");
        if (commaPos == string::npos) {
          break;
        }
        // get the word out of the token
        string word = token.substr(0, commaPos);
        int value = stoi(token.substr(commaPos+1, rightParen));
        auto it = result.find(word);
        if (it != result.end()){
          result[word] += value;
        }else{
          // if the key does not exists
          result[word] = value;
        }
        leftParen = line.find("(", rightParen+1);
      }
  }

  exportResult(result, outputDir);
  writeSuccess(fileManager, outputDir);
}

string generateRandomId(int length) {

  std::string id;
  static const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::random_device random_device;
  std::mt19937 rng(random_device());
  std::uniform_int_distribution<int> distribution(0, sizeof(characters) - 2);

  for (int i = 0; i < length; ++i) {
    id += characters[distribution(rng)];
  }

  return id;
}

extern "C" void processSortResult(string inputFilePath, string tempDir) {

  // inputFilePath should be a single file produced by the mapper process.
  // the sort class should sort this file first
  string randomID = generateRandomId(9);
  string sortResultFilePath = tempDir + "/sort/" + randomID + ".txt";
  Sort sort = Sort(inputFilePath, sortResultFilePath);
  sort.Sorter();

  FileManager fileManager;
  string reduceTempOutputFileName = "/" + generateRandomId(10) + ".txt";

  // Parse the intermediate file produced by the sort class
  array<string, 2> inputFile = fileManager.readFile(sortResultFilePath);
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

    reduce(word, onesList, tempDir+"/reduce", reduceTempOutputFileName);
    leftParen = line.find("(", rightParen+1);

  }

}
