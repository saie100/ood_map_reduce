/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 * Copyright [2023] <Copyright Varis Nijat>
 **/

#include "Reduce.h"

#include <list>
#include <string>

using std::list;
using std::string;

/**
 * Class Constructor specifying output directory
 */
Reduce::Reduce(string outputDir) { output_dir = outputDir; }

void Reduce::redue(string key, list<int> intIterator) {
  // Sum all the values in the list
  int sum = 0;
  for (int i : intIterator) {
    sum += i;
  }

  export_result(key, sum);
}

void Reduce::export_result(string key, int value) {}
