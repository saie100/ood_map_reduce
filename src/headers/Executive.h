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

  // return list of arguments
  vector<string> getArguments();

 private:
  /***************************** Members **************************************/
  int argumentCount; // stores the number of arguments
  vector<string> arguments; // stores a list of arguments
  /****************************************************************************/
  
  /***************************** Methods **************************************/
  // Add arguements to the member variable list "arguments"
  void setArguments(int argumentCount, char **arguments);
  /****************************************************************************/

};

#endif
