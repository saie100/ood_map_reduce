#include "headers/Reduce.h"
#include "headers/Workflow.h"

#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::vector;

int main() {
    
    Workflow wf = Workflow("data/input/", "data/temp/", "data/output");

    wf.start();

    return 0;
}
