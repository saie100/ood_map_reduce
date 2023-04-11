/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
**/

#include "Reduce.h"

#include <string>
#include <list>

using std::string;
using std::list;

/**
 * Class Constructor specifying output directory
*/
Reduce::Reduce(string outputDir){
    output_dir = outputDir;
};

void Reduce::redue(string& key, list<int> intIterator){

    // Sum all the values in the list
    int sum = 0;
    for (int i: intIterator){
        sum += i;
    }
}


void Reduce::export_result(string& key, int value){

}
