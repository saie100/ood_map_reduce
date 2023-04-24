/**
 * @file Reduce.cpp
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
 **/

#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <string>

using std::string;

class Workflow{

    private:
        string inputDir;
        string outputDir;
        string tempDir;

    public:
        Workflow(string input_dir, string temp_dir, string output_dir);
        void start();
};

#endif
