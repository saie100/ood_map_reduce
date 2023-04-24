#include "headers/Executive.h"
#include "headers/FileManager.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

// Inside the constructor, we initialize the member variable "argumentCount"
// and call setArguements to initialize the member variable "arguments"
// with the value in char **arguments  
Executive::Executive(int argumentCount, char **arguments) {
  
  this->argumentCount = argumentCount; // initialize member variable "argumentCount"
  setArguments(this->argumentCount, arguments); // initialize member variable "arguemnts"
};

// Add arguements to the member variable list "arguments"
void Executive::setArguments(int count, char **auguments) {
  
  // exit program if there are not 3 arguments
  if (count == 1) {
    cerr << "Error: No arguments entered, need 3\nHelp: main [input dir] [temp dir] [output dir]" << endl;
    exit(1);
  }
  else if (count == 2) {
    cerr << "Error: 1 arguments entered, need 3\nHelp: main [input dir] [temp dir] [output dir]" << endl;
    exit(1);
  }
  else if (count == 3) {
    cerr << "Error: 2 arguments entered, need 3\nHelp: main [input dir] [temp dir] [output dir]" << endl;
    exit(1);
  }
  
  // Iterate arguments pointer and store arguements in 
  // member variable list "arguments"
  for (int i = 1; i < count; i++) {
    // check if the arguement at specified index is a valid directory 
    if (FileManager::isValid(FileManager::DIR, auguments[i])) {
      this->arguments.push_back(auguments[i]); // add argument to list
    }
  }
};

// returns member variable list "arguments"
vector<string> Executive::getArguments() {
  return this->arguments;
}
