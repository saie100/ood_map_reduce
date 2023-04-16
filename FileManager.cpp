#include "FileManager.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

FileManager::FileManager(){

};

bool FileManager::isValid(string path) {
  ifstream file;
  file.open(path);
  if (file) {
    file.close();
    return true;
  } else {
    return false;
  }
};

array<string, 2> FileManager::readFile(string input_path) {
  if (FileManager::isValid(input_path)) {
  } else {
  }
};

bool FileManager::writeFile(string output_path, string content){

};
