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
#include <vector>
#include <mutex>

using std::string;
using std::vector;
using std::mutex;

class Workflow{
    public:
        using ThreadStatus = struct {
            int threadId;
            bool done;
        };

        using Stub = struct {
            int port_num;
            vector<ThreadStatus> threads;
        };

        static const int controller_port = 9000;

        Workflow(string input_dir, string temp_dir, string output_dir, string reduce_dll_path, string map_dll_path, int proc_num);
        void start();
        static void resetStubs();
        static void setStubDone(int threadId, bool done);
        static bool stubsAreDone();

    private:
        string inputDir;
        string outputDir;
        string tempDir;
        string reduceDllPath;
        string mapDllPath;
        int procNum;
        static vector<Stub> stubs;
        static mutex stub_locker;
};

#endif



