/**
 * @file Reduce.h
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 * Copyright [2023]
 **/

#ifndef REDUCE_H
#define REDUCE_H

#include <vector>
#include <string>
#include "FileManager.h"

using std::vector;
using std::string;

class Reduce {
 public:
  /**
   * Class Constructor specifying output directory
   */
  explicit Reduce(string input_file_path, string output_dir);

  /** 
   * @param key string, a distinct word
   * @param intIterator list<int>, an iterator of integers
  */
  void reduce(string key, vector<int> intIterator);

  bool processSortResult();

 private:
  string inputFilePath;
  string outputDir;
  FileManager fileManager;

  /** 
   * Write result to a text file
   * @param key string, a distinct word
   * @param intIterator int, number of occurrence
  */
  void exportResult(string key, int value);

  /**
   * Write SUCCESS file to the output directory
  */
  void writeSuccess();

};

#endif
