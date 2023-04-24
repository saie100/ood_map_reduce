#include "headers/Reduce.h"
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::vector;

int main() {
    
    Reduce r = Reduce("data/temp/test.txt", "data/output/");
    bool successfulProcess = r.processSortResult();

    return 0;
}
