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

// Inside the constructor, we initialize the member variables
// and set variables equal to the values in char **arguments  
Executive::Executive(int argumentCount, char **arguments) {

  // initialize member variables
  this->inputDir = "";
  this->tempDir = "../test_data/temp_data/"; // default temp directory
  this->outputDir = "";
  this->reduceDLL = "";
  this->mapDLL = "";
  
  // loop through arguments and set arguments equal to
  // member variables based on keyword arguments 
  for(int i=0; i < argumentCount; i++){
    string kwarg = string(arguments[i]);

    if(kwarg == "--inputDir"){
      string nextArg = string(arguments[i+1]);

      // check if next argument is a valid directory
      if(FileManager::isValid(FileManager::DIR, nextArg)){
        this->inputDir = arguments[i+1];
      }
    }
    else if(kwarg == "--tempDir"){
      string nextArg = string(arguments[i+1]);

      // check if next argument is a valid directory
      if(FileManager::isValid(FileManager::DIR, nextArg)){
        this->tempDir = arguments[i+1];
      }
    }
    else if(kwarg == "--outputDir"){
      string nextArg = string(arguments[i+1]);

      // check if next argument is a valid directory
      if(FileManager::isValid(FileManager::DIR, nextArg)){
        this->outputDir = arguments[i+1];
      }
    }
    else if(kwarg == "--reduceDLL"){
      string nextArg = string(arguments[i+1]);

      this->reduceDLL = arguments[i+1];
      // }
    }
    else if(kwarg == "--mapDLL"){
      string nextArg = string(arguments[i+1]);

      // check if next argument is a valid directory
      if(FileManager::isValid(FileManager::DIR, nextArg)){
        this->mapDLL = arguments[i+1];
      }
    }
  }

  // if keyword argument was not provided for inputDir, return error
  if(this->inputDir == ""){
    cerr << "--inputDir keyword argument required" << endl;
    exit(1);
  }
  // if keyword argument was not provided for outputDir, return error
  else if(this->outputDir == ""){
    cerr << "--outputDir keyword argument required" << endl;
    exit(1);
  }
  // if keyword argument was not provided for reduceDLL, return error
  else if(this->reduceDLL == ""){
    cerr << "--reduceDLL keyword argument required" << endl;
    exit(1);
  }
  // if keyword argument was not provided for mapDir, return error
  else if(this->mapDLL == ""){
    cerr << "--mapDLL keyword argument required" << endl;
    exit(1);
  }

}

// returns inputDir member variable
string Executive::getInputDir(){
  return this->inputDir;
}

// returns tempDir member variable
string Executive::getTempDir(){
  return this->tempDir;
}

// returns outputDir member variable
string Executive::getOutputDir(){
  return this->outputDir;
}

// returns reduceDLL member variable
string Executive::getReduceDLL(){
  return this->reduceDLL;
}
// returns mapDLL member variable
string Executive::getMapDLL(){
  return this->mapDLL;
}



