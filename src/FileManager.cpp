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

FileManager::FileManager(){

};

bool FileManager::isValid(string type, string path) {
  struct stat sb;
  const char* p = path.c_str();

  if (stat(p, &sb) == 0) {
    if (type == "file") {
      if (sb.st_mode & S_IFREG) {
        return true;
      } else {
        return false;
      }
    } 
    else if (type == "dir") {
      if (sb.st_mode & S_IFDIR) {
        return true;
      } else {
        return false;
      }
    } 
    else {
      cerr << "Error: Please enter `file` or `dir` for `type` argument" << endl;
      exit(1);
    }
  } else {
    cerr << "Error Invalid Path: " << path << endl;
    exit(1);
  }
};

string FileManager::getFilename(string path) {
  if (FileManager::isValid("file", path)) {
    if (path.find("/") != string::npos) {
      const string delimitter = "/";  // for linux (forward slash)
      vector<string> list;
      size_t current_index = 0;
      size_t count = 1;

      while (current_index < path.length()) {
        size_t new_index = path.find(delimitter.c_str(), current_index, 1);
        if (new_index == string::npos) {
          string file_name = path.substr(current_index);
          list.push_back(file_name);  // store filename in list
          break;
        }
        string dir_name =
            path.substr(current_index, (new_index - current_index) + 1);
        list.push_back(dir_name);       // store dir_name in list
        current_index = new_index + 1;  // increment current index
      }

      return list.back();
    } else if (path.find("\\") != string::npos) {
      const string delimitter = "\\";  // for windows (back slash)
      vector<string> list;
      size_t current_index = 0;
      size_t count = 1;

      while (current_index < path.length()) {
        size_t new_index = path.find(delimitter.c_str(), current_index, 1);
        if (new_index == string::npos) {
          string file_name = path.substr(current_index);
          list.push_back(file_name);  // store filename in list
          break;
        }
        string dir_name =
            path.substr(current_index, (new_index - current_index) + 1);
        list.push_back(dir_name);       // store dir_name in list
        current_index = new_index + 1;  // increment current index
      }

      return list.back();
    }

    else {
      return "";
    }
  } else {
    return "";
  }
}

array<string, 2> FileManager::readFile(string input_path) {
  array<string, 2> arr = {"", ""};

  if (FileManager::isValid("file", input_path)) {
    ifstream file(input_path);
    if (file.is_open()) {
      string data = "";
      string line = "";
      while (file) {
        getline(file, line);
        data += line;
      }

      arr[0] = FileManager::getFilename(input_path);
      arr[1] = data;
      return arr;
    } else {
      return arr;
    }
  } else {
    return arr;
  }
};

bool FileManager::writeFile(string mode, string output_path, string filename,
                            string content) {
  if (FileManager::isValid("dir", output_path)) {
    if (mode == "w") {
      ofstream file(output_path + filename, ios::trunc);  // overwrite mode
      if (file.is_open()) {
        file << content;
        file.close();
        return true;
      } else {
        return false;
      }
    } else if (mode == "a") {
      ofstream file(output_path + filename, ios::app);  // append mode
      if (file.is_open()) {
        file << content;
        file.close();
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

vector<string> FileManager::getFilesFromDir(string dir_path) {
  vector<string> list;

  if (FileManager::isValid("dir", dir_path)) {
    for (const auto& entry : fs::directory_iterator(dir_path)) {
      fs::path outfilename = entry.path();
      string outfilename_str = outfilename.string();
      if (FileManager::isValid("file", outfilename_str)) {
        list.push_back(outfilename_str);
      }
    }
    return list;

  } else {
    cerr << "Error: Invalid Directory Path\n";
    exit(1);
    return list;
  }
}
