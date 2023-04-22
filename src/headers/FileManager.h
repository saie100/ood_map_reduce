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
  static bool isValid(string type, string path);

  // returns filename given a path to a file
  string getFilename(string path);

  // returns an array of string. First index is the name of the file
  // the second index is the content of the file
  array<string, 2> readFile(string inputPath);

  // returns true if file was successfully written to output_dir. Else returns
  // false
  bool writeFile(string mode, string outputDir, string filename,
                 string content);

  // returns a list of files inside a directory
  // parameter dir_path is the path to a directory
  vector<string> getFilesFromDir(string dirPath);

 private:
};

#endif
