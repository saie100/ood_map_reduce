/*
Map Class
Accepts raw data from a file, removes whitespace, punctuation, and capitalization, and tokenizes the data into distinct words.
Periodically outputs data to a temporary file.
*/

#include "Map.hpp"
#include <cctype>
#include <algorithm>

#ifdef TEST_STUB
#include <iostream>
using std::cout;
#endif

using std::find_if;
using std::distance;

// default constructor
Map::Map() {}

// checks if the passed in character is a punctuation or whitespace character
bool isPunctOrSpace(int aChar) {
    return (ispunct(aChar) || isspace(aChar));
}

// tokenizes the value into distinct words, removing whitespace, punctuation, and capitalization
void Map::map(const string& aKey, const string& aValue) {
    string tempValue = aValue;
    string outputBuffer;
    // loop through the value buffer, parsing words, buffering and exporting output data
    while (tempValue.size() > 0) {
        auto it = find_if(tempValue.begin(), tempValue.end(), isPunctOrSpace);
        auto foundIdx = distance(tempValue.begin(), it);
        string subStr{};
        if (foundIdx == 0) {
            // punctuation or space found at index 0, erase it and continue
            tempValue.erase(0, 1);
        } else {
            // punctuation or space found, grab the string
            subStr = tempValue.substr(0, foundIdx);
            // add the (word, 1) pair to the output buffer
            outputBuffer += "(";
            // loop through the sub string, adding the word as a lowercase string to the output buffer
            for (char c : subStr) {
                outputBuffer += static_cast<char>(tolower(c));
            }
            outputBuffer += ", 1)";
            // erase up to and including the whitespace, then continue
            tempValue.erase(tempValue.begin(), it + 1);
        }

#ifdef TEST_STUB
        cout << "output buffer: " << outputBuffer << "\n";
#endif

        // if the output buffer has over 100 characters in it, export the data to the temporary file
        if (outputBuffer.size() > 100) {
            exportData(aKey, outputBuffer);
        }
    }
}

// exports tokenized values to the disk
void Map::exportData(const string& aKey, const string& aValue) {
    // TODO export the data when we have the FileManagement class
}

#ifdef TEST_STUB
int main() {
    Map mapguy;
    string testStr = "This here is a string, go ahead and buffer this.";
    string fileName = "file";
    mapguy.map(fileName, testStr);
}
#endif