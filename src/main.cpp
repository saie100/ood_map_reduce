#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "headers/Executive.h"
#include "headers/Workflow.h"
#include "headers/FileManager.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::array;

// main function
int main(int argc, char ** argv){

  Executive commandline(argc, argv);
  FileManager::deleteFilesFromDir(commandline.getTempDir());
  FileManager::deleteFilesFromDir(commandline.getOutputDir());
  Workflow wf(
    commandline.getInputDir(),
    commandline.getTempDir(),
    commandline.getOutputDir(),
    commandline.getReduceDLL(),
    commandline.getMapDLL(),
    commandline.getProcNum()
  );
  wf.start();
  
  return 0;
}