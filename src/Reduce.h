/**
 * @file Reduce.h
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 * Copyright [2023] <Copyright Varis Nijat>
 **/

#ifndef REDUCE_H
#define REDUCE_H

#include <list>
#include <string>

using std::list;
using std::string;

class Reduce {
 public:
  /**
   * Class Constructor
   */
  explicit Reduce(string outputDir);

  void redue(string key, list<int> intIterator);

 private:
  string output_dir;

  void export_result(string key, int value);
};

#endif
