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
  setArgumentValue(argument_count, argv);
};

void Executive::setArgumentValue(int count, char **auguments) {
  if (count == 1) {
    cout << "Error: Please enter a directory path" << endl;
    exit(1);
  }
  for (int i = 0; i < count; i++) {
    argument_value.push_back(auguments[i]);
  }
};

vector<string> Executive::getArguments() {
  return vector<string>(argument_value.begin() + 1, argument_value.end());
}