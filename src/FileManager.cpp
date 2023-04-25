#include "headers/FileManager.h"
#include <sys/stat.h>
#include <sys/types.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
using std::vector;
namespace fs = std::filesystem;

// constructor
FileManager::FileManager(){};

// returns true if file/directory exists, else returns false
// path is the path to the file of directory
// type is either FILE or DIR
bool FileManager::isValid(TYPE type, string path) {
  struct stat sb; // initialize a stat struct, used for reading system filesystem
  const char* p = path.c_str(); // convert "path" from string into char pointer

  // stat returns 0 when path given is a valid path on computer filesystem
  if (stat(p, &sb) == 0) {
    if (type == FILE) {
      // bitwise operation on stat struct,  
      // returns 1 if "path" is a file, else return 0
      if (sb.st_mode & S_IFREG) {
        return true;
      } else {
        return false;
      }
    } 
    else if (type == DIR) {
      // bitwise operation on stat struct,  
      // returns 1 if "path" is a directory, else return 0
      if (sb.st_mode & S_IFDIR) {
        return true;  
      } else {
        return false;
      }
    } 
    // else "type" was not properly specified
    else {
      cerr << "Error: Please enter FileManager::FILE or FileManager::DIR for `type` argument" << endl;
      exit(1);
    }
  }
  // else "path" given is an invalid path on computer filesystem
  else {
    cerr << "Error invalid path: " << path << endl;
    exit(1);
  }
};

// returns filename given a path to a file
string FileManager::getFilename(string path) {
  // check if the path is a valid file path
  if (FileManager::isValid(FILE, path)) {

    // checks if there is a forward-slash in the path
    if (path.find("/") != string::npos) {
      const string delimitter = "/";  // for linux (forward-slash)
      size_t current_index = 0;

      while (current_index < path.length()) {
        // path.find returns an index given a demlimitter. search starts at the starting index 
        size_t new_index = path.find(delimitter.c_str(), current_index, 1);
        // if new_index is not found
        if (new_index == string::npos) {
          return path.substr(current_index);
        }
        // set current_index to new_index + 1
          current_index = new_index + 1; 
      }

    }
    // checks if there is a backslash in the path 
    else if (path.find("\\") != string::npos) {
      const string delimitter = "\\";  // for windows (backslash)
      size_t current_index = 0;

      while (current_index < path.length()) {
        // path.find returns an index given a demlimitter. search starts at the starting index 
        size_t new_index = path.find(delimitter.c_str(), current_index, 1);
        if (new_index == string::npos) {
          return path.substr(current_index);
        }
        // set current_index to new_index + 1
        current_index = new_index + 1; 
      }
    }
    // else if there is no backslash or forward-slash in the path, we return an empty string
    else {
      return "";
    }  
  } 
  // else if the path is not a valid file path, we return an empty string 
  else {
    return "";
  }
  return "";
}

// returns an array of strings given a path to a file 
// first index is the name of the file
// the second index is the content of the file
array<string, 2> FileManager::readFile(string inputPath) {
  
  array<string, 2> arr = {"", ""}; // initializing array that will be returned later
  // check if the inputPath is a valid file path
  if (FileManager::isValid(FILE, inputPath)) {
    // opens file given path to file
    ifstream file(inputPath);
    // if the file is open 
    if (file.is_open()) {
      string data = "";
      string line = "";
      // while we are not at the end of the file
      while (getline(file, line)) {
        data += line;
      }
      // store the name of the file in the first index
      arr[0] = FileManager::getFilename(inputPath);
      // store the content of the file in the second index
      arr[1] = data;
      return arr; // return array
    }
    // else if file do not open
    // return empty array
    else {
      return arr;
    }
  } 
  // else if the path is not a valid file path, we return an empty string  
  else {
    return arr;
  }
};

// given the mode (CREATE, APPEND), path to output directory, the filename,
// and content of the file. This method returns true if file was 
// successfully written to outputDir, else returns false 
bool FileManager::writeFile(MODE mode, string outputPath, string filename, string content) {
  return FileManager::writeFile(mode, outputPath+filename, content);
}

// given the mode (CREATE, APPEND), the file path, and content of the file. 
// This method returns true if file was successfully written, else returns false 
bool FileManager::writeFile(MODE mode, string filePath, string content) {
  // check if filePath is a valid file path
  if (FileManager::isValid(FILE, filePath)) {
    if (mode == CREATE) {
      // open file in overwrite mode  
      ofstream file(filePath, ios::trunc);  // overwrite mode
      // if file is open
      if (file.is_open()) {
        // write content to file
        file << content;
        file.close();
        return true;
      }
      // else if file is not open 
      // return false 
      else {
        return false;
      }
    } 
    else if (mode == APPEND) {
      // open file in append mode
      ofstream file(filePath, ios::app);  // append mode
      // if file is open
      if (file.is_open()) {
        // write content to file
        file << content;
        file.close();
        return true;
      }
      // else if file is not open 
      // return false   
      else {
        return false;
      }
    }
    // else mode was not properly set 
    // return false  
    else {
      return false;
    }
  }
  // else if the path is not a valid directory path, return false    
  else {
    return false;
  }
}

// returns a list of files inside a directory
// parameter dirPath is the path to the directory
vector<string> FileManager::getFilesFromDir(string dirPath) {
  // list of files to be returned
  vector<string> list;
  // check if the dirPath is a valid directory path
  if (FileManager::isValid(DIR, dirPath)) {
    // iterates through the files and sub-directories within the directory (dirPath)
    for (const auto& entry : fs::directory_iterator(dirPath)) {
      fs::path outfilename = entry.path(); // retrieves a path
      string outfilename_str = outfilename.string(); // converts the path into a string
      // check if the path is a valid file path
      if (FileManager::isValid(FILE, outfilename_str)) {
        list.push_back(outfilename_str); // adds file path to list
      }
    }
    return list;

  }
  // else if the dirPath is not a valid directory path
  // exit program
  else {
    cerr << "Error: Invalid Directory Path\n";
    exit(1);
  }
}