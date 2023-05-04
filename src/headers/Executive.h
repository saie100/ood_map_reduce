#ifndef Executive_H
#define Executive_H

#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Executive {
 public:


  // Inside the constructor, we initialize the member variable "argumentCount"
  // and call setArguements to initialize the member variable "arguments"
  // with the value in char **arguments  
  Executive(int argumentCount, char **arguments);

  // returns inputDir member variable
  string getInputDir();
  
  // returns tempDir member variable
  string getTempDir();
  
  // returns outputDir member variable
  string getOutputDir();

  // returns reduceDLL member variable
  string getReduceDLL();

  // returns mapDLL member variable
  string getMapDLL();

 private:
  /***************************** Members Variables **************************************/
  string inputDir;
  string tempDir;
  string outputDir;
  string reduceDLL;
  string mapDLL;

  /****************************************************************************/
  
};

#endif
