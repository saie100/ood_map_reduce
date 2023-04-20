/*
Map Class
Accepts raw data from a file, removes whitespace, punctuation, and capitalization, and tokenizes the data into distinct words.
Periodically outputs data to a temporary file.
*/

#ifndef MAP_HPP
#define MAP_HPP

#include <string>

using std::string;

class Map
{
public:
    // default constructor
    Map();

    // tokenizes the value into distinct words, removing whitespace, punctuation, and capitalization
    void map(const string& aKey, const string& aValue);

    // exports tokenized values to the disk
    void exportData(const string& aKey, const string& aValue);
};

#endif