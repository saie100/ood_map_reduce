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
  Workflow wf(
    commandline.getInputDir(),
    commandline.getTempDir(),
    commandline.getOutputDir(),
    commandline.getReduceDLL(),
    commandline.getMapDLL()
  );
  wf.start();
  
  return 0;
}