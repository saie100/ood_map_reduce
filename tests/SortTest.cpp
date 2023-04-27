#include <gtest/gtest.h>
#include "../src/headers/Sort.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

TEST(SortTest, EqualCounts){

    Sort sort("./in.txt", "./out.txt");
    
    std::ofstream infile("./in.txt");
    infile << "(\"hey\",1)(\"this\",1)(\"is\",1)(\"a\",1)(\"string\",1)(\"with\",1)(\"no\",1)(\"repeating\",1)(\"words\",1)";
    infile.close();

    sort.Sorter();
    
    std::ifstream outfile("./out.txt");
    string fileData;
    if (outfile.is_open()) {
        getline(outfile, fileData);
        outfile.close();
    }

    ASSERT_EQ(fileData, "(\"a\", [1])(\"hey\", [1])(\"is\", [1])(\"no\", [1])(\"repeating\", [1])(\"string\", [1])(\"this\", [1])(\"with\", [1])(\"words\", [1])");

    remove("./in.txt");
    remove("./out.txt");
}

TEST(SortTest, repeatedWords){

    Sort sort("./in.txt", "./out.txt");
    
    std::ofstream infile("./in.txt");
    infile << "(\"hey\",1)(\"hey\",1)(\"is\",1)(\"is\",1)(\"is\",1)(\"with\",1)(\"with\",1)(\"with\",1)(\"with\",1)";
    infile.close();

    sort.Sorter();
    
    std::ifstream outfile("./out.txt");
    string fileData;
    if (outfile.is_open()) {
        getline(outfile, fileData);
        outfile.close();
    }

    ASSERT_EQ(fileData, "(\"with\", [1, 1, 1, 1])(\"is\", [1, 1, 1])(\"hey\", [1, 1])");

    remove("./in.txt");
    remove("./out.txt");
}

TEST(SortTest, repeatedAndEqualCounts){

    Sort sort("./in.txt", "./out.txt");
    
    std::ofstream infile("./in.txt");
    infile << "(\"some\",1)(\"of\",1)(\"these\",1)(\"words\",1)(\"will\",1)(\"repeat\",1)(\"and\",1)(\"some\",1)(\"will\",1)(\"not\",1)(\"repeat\",1)"
           << "(\"will\",1)(\"you\",1)(\"be\",1)(\"able\",1)(\"to\",1)(\"sort\",1)(\"some\",1)(\"of\",1)(\"them\",1)(\"or\",1)(\"all\",1)"
           << "(\"of\",1)(\"them\",1)";
    infile.close();

    sort.Sorter();
    
    std::ifstream outfile("./out.txt");
    string fileData;
    if (outfile.is_open()) {
        getline(outfile, fileData);
        outfile.close();
    }

    string checkStr = "(\"of\", [1, 1, 1])(\"some\", [1, 1, 1])(\"will\", [1, 1, 1])(\"repeat\", [1, 1])(\"them\", [1, 1])(\"able\", [1])(\"all\", [1])";
           checkStr += "(\"and\", [1])(\"be\", [1])(\"not\", [1])(\"or\", [1])(\"sort\", [1])(\"these\", [1])(\"to\", [1])(\"words\", [1])(\"you\", [1])";
    ASSERT_EQ(fileData,  checkStr);

    remove("./in.txt");
    remove("./out.txt");
}