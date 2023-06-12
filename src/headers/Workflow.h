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
        string reduceDllPath;
        string mapDllPath;
        int procNum;

    public:
        Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path, int proc_num);
        // void mapProcess(int threadId);
        void start();

        static const int controller_port = 9000;
};

#endif



