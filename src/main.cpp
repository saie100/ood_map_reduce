#include "Reduce.h"
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::vector;

int main() {
    vector<int> my_list;
    my_list.push_back(1);
    my_list.push_back(2);
    my_list.push_back(3);
    my_list.push_back(4);
    my_list.push_back(5);
    string tempDir = "data";

    Reduce reducer = Reduce(tempDir);
}
