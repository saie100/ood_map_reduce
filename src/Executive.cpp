#include "headers/Executive.h"
#include "headers/FileManager.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

Executive::Executive(int argumentCount, char **arguments) {
  this->argumentCount = argumentCount;
  setArgumentValue(this->argumentCount, arguments);
};

void Executive::setArgumentValue(int count, char **auguments) {
  if (count == 1) {
    cerr << "Error: Please enter a directory path" << endl;
    exit(1);
  }
  for (int i = 1; i < count; i++) {
    if (FileManager::isValid("dir", auguments[i])) {
      this->arguments.push_back(auguments[i]);
    }
  }
};

vector<string> Executive::getArguments() {
  return vector<string>(this->arguments.begin(), this->arguments.end());
}
