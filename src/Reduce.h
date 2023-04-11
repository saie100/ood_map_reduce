/**
 * @file Reduce.h
 * @author: Varis Nijat
 * CSE 687 Object Oriented Design in C++
 * Syracuse University
 * Project MapReduce
**/

#ifndef REDUCE_H
#define REDUCE_H

#include <string>
#include <list>

using std::string;
using std::list;

class Reduce{

    public:

        /**
         * Class Constructor 
        */
        Reduce(string outputDir);

        void redue(string& key, list<int> intIterator);

    private:

        string output_dir;

        void export_result(string& key, int value);
};

#endif
