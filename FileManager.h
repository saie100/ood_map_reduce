#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class FileManager {
public:
  // constructor
  FileManager();

  // returns true if directory exists. Else returns false
  bool isValid(string path);

  // returns an array of string. First index is the name of the file
  // the second index is the content of the file
  array<string, 2> readFile(string input_path);

  // returns true if file was successfully written to path. Else returns false
  bool writeFile(string output_path, string content);

private:
};

#endif