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
    Map(const string& aOutputPath);

    // tokenizes the value into distinct words, removing whitespace, punctuation, and capitalization
    void map(const string& aKey, const string& aValue);

private:
    // exports tokenized values to the disk
    void exportData(const string& aKey, const string& aValue);

    string outputPath; // holds the output path for the exportData function

};

#endif