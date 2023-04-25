#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "src/headers/Executive.h"
#include "src/headers/FileManager.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::array;

// main function
int main(int argc, char ** argv){

  // We need a seperate main function.
  // Placing main in Executive.cpp prevents me from running my unit test
  Executive commandline(argc, argv);

  /*****Call Work Flow and pass it arguments as parameters ****/
  // Workflow.start(commandline.getArguments)

  return 0;
}