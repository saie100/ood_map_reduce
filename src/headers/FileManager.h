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

  enum MODE {APPEND = 0, CREATE = 1};
  enum TYPE {FILE = 0, DIR = 1};  

  // returns true if file/directory exists, else returns false
  // path is the path to the file of directory
  // type is the either FILE or DIR
  static bool isValid(TYPE type, string path);

  // returns filename given a path to a file
  static string getFilename(string path);

  // returns an array of strings given a path to a file 
  // first index is the name of the file
  // the second index is the content of the file
  static array<string, 2> readFile(string inputPath);

  // given the mode (CREATE, APPEND), path to output directory, the filename,
  // and content of the file. This method returns true if file was 
  // successfully written to outputDir, else returns false 
  static bool writeFile(MODE mode, string outputDir, string filename,
                 string content);

   // given the mode (CREATE, APPEND), the file path, and content of the file. 
   // This method returns true if file was successfully written, else returns false 
  static bool writeFile(MODE mode,string filePath, string content);

  // returns a list of files inside a directory
  // parameter dirPath is the path to the directory
  static vector<string> getFilesFromDir(string dirPath);

  // deletes all text files within a directory
  // parameter dirPath is the path to the directory
  static void deleteFilesFromDir(string dirPath);

 private:
};

#endif
