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
        // check if we found an apostrophe
        if (*it == '\'') {
            // check the character after the apostrophe
            if (*(it + 1) == 's') {
                // we found an s after the apostrophe, erase the apostrophe and the s
                tempValue.erase(it , it + 2);
            } else {
                // erase the apostrophe, then continue the loop to capture the word without the apostrophe
                tempValue.erase(it , it + 1);
                continue;
            }
        }
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
            // erase the word from tempValue
            tempValue.erase(tempValue.begin(), it);
            if (tempValue.size() > 0) {
                // erase the whitespace or punctuation, and trailing s if necessary, then continue
                tempValue.erase(0, 1);
            }
        }

#ifdef TEST_STUB
        cout << "output buffer: " << outputBuffer << "\n";
#endif

        // if the output buffer has over 100 characters in it, export the data to the temporary file
        if (outputBuffer.size() > 100) {
            exportData(aKey, outputBuffer);
            outputBuffer.clear();
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
    string testStr = "This here is a string's string, go ahead and buffer this. You're probably going to see who's got apostrophes";
    string fileName = "file";
    mapguy.map(fileName, testStr);
    string allswell = "ACT I\n";
    allswell += "SCENE I. Rousillon. The COUNT's palace.\n\n";
    allswell += "Enter BERTRAM, the COUNTESS of Rousillon, HELENA, and LAFEU, all in black\n";
    allswell += "COUNTESS\n";
    allswell += "In delivering my son from me, I bury a second husband.\n";
    allswell += "BERTRAM\n";
    allswell += "And I in going, madam, weep o'er my father's death\n";
    allswell += "anew: but I must attend his majesty's command, to\n";
    allswell += "whom I am now in ward, evermore in subjection.\n";
    allswell += "LAFEU\n";
    allswell += "You shall find of the king a husband, madam; you,\n";
    allswell += "sir, a father: he that so generally is at all times\n";
    allswell += "good must of necessity hold his virtue to you; whose\n";
    allswell += "worthiness would stir it up where it wanted rather\n";
    allswell += "than lack it where there is such abundance.\n";
    allswell += "COUNTESS\n";
    allswell += "What hope is there of his majesty's amendment?\n";
    allswell += "LAFEU\n";
    allswell += "He hath abandoned his physicians, madam; under whose\n";
    allswell += "practises he hath persecuted time with hope, and\n";
    allswell += "finds no other advantage in the process but only the\n";
    allswell += "losing of hope by time.\n";
    allswell += "COUNTESS\n";
    allswell += "This young gentlewoman had a father,--O, that\n";
    allswell += "'had'! how sad a passage 'tis!--whose skill was\n";
    allswell += "almost as great as his honesty; had it stretched so\n";
    allswell += "far, would have made nature immortal, and death\n";
    allswell += "should have play for lack of work. Would, for the\n";
    allswell += "king's sake, he were living! I think it would be\n";
    allswell += "the death of the king's disease.\n";
    allswell += "LAFEU\n";
    allswell += "How called you the man you speak of, madam?\n";
    allswell += "COUNTESS\n";
    allswell += "He was famous, sir, in his profession, and it was\n";
    allswell += "his great right to be so: Gerard de Narbon.\n";

    fileName = "all's well that end's well";
    mapguy.map(fileName, allswell);
}
#endif