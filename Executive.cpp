#include "Executive.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

Executive::Executive(int argc, char **argv) {

  this->argument_count = argc;
  setArgumentArray(argument_count, argv);
};

void Executive::setArgumentArray(int count, char **values) {

  for (int i = 0; i < count; i++) {
    argument_value.push_back(values[i]);
    cout << argument_value[i] << endl;
  }
};
