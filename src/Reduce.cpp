/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 **/

#include "Reduce.h"
#include "FileManager.h"

#include <string>
#include <vector>

using std::string;
using std::vector;
using std::to_string;

/**
 * Class Constructor specifying output directory
 */
Reduce::Reduce(string output_dir):outputDir(output_dir){}

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

  // if we are going with initial letter approach
  char initialLetter = key.at(0);
  string fileName = initialLetter + ".txt";

  // if we are going with single output file
  string fileName = "output.txt";

  bool isSuccessfulWrite = fm.writeFile("a", outputDir, fileName, content);

}

void Reduce::writeSuccess(){
  FileManager fm = FileManager();
  bool isSuccessfulWrite = fm.writeFile("a", outputDir, "SUCCESS.txt", "SUCCESS");
}
