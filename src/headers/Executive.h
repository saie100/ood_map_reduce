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
  Executive(int argument_count, char **argument_values);

  // get argument value
  vector<string> getArguments();

 private:
  /******* Members ******/
  int argument_count;
  vector<string> argument_value;

  /***** Methods *****/

  // sets argument value
  void setArgumentValue(int argument_count, char **arguments);
};

#endif