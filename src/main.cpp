#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "headers/Executive.h"
#include "headers/Workflow.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::array;

// main function
int main(int argc, char ** argv){

  Executive commandline(argc, argv);
  vector<string> files = commandline.getArguments();
  Workflow wf(files[0], files[1], files[2]);
  wf.start();
  
  return 0;
}