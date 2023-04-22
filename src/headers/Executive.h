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
  // constructor
  Executive(int argumentCount, char **arguments);

  // get argument value
  vector<string> getArguments();

 private:
  /******* Members ******/
  int argumentCount;
  vector<string> arguments;

  /***** Methods *****/

  // sets argument value
  void setArgumentValue(int argumentCount, char **arguments);
};

#endif
