/*
Map Class
Accepts raw data from a file, removes whitespace, punctuation, and capitalization, and tokenizes the data into distinct words.
Periodically outputs data to a temporary file.
*/

#include "headers/Map.hpp"
#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <array>

// #define TEST_STUB
#ifdef TEST_STUB
#include <iostream>
using std::cout;
#endif

using std::array;
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
        string contractionStr1{};
        string contractionStr2{};
        auto foundIdx = distance(tempValue.begin(), it);
        // check if we found an apostrophe
        if (*it == '\'') {
            // check the character after the apostrophe
            if (*(it + 1) == 's') {
                // we found an s after the apostrophe, check if it is an "is" contraction such as "it's"
                string tempStr{tempValue.substr(0, foundIdx)};
                string compareStr{};
                bool isContraction{false};
                // create the lowercase comparison string
                for (char c : tempStr) {
                    compareStr += static_cast<char>(tolower(c));
                }
                // compare to common contractions for 's
                if (compareStr == "it" || compareStr == "he"|| compareStr == "she" || compareStr == "who" || compareStr == "what"
                    || compareStr == "when" || compareStr == "where"|| compareStr == "why" || compareStr == "how" || compareStr == "which"
                    || compareStr == "this" || compareStr == "that"|| compareStr == "here" || compareStr == "there" || compareStr == "somebody"
                    || compareStr == "someone" || compareStr == "something") {
                    // this is a contraction, set up the strings
                    contractionStr1 = compareStr;
                    contractionStr2 = "is";
                    isContraction = true;
                }
                if (isContraction) {
                    // erase the contraction from tempValue
                    tempValue.erase(tempValue.begin(), it + 2);
                } else {
                    // erase the apostrophe and the s since this is a possessive
                    tempValue.erase(it , it + 2);
                }
            } else if (*(it + 1) == 'r' && *(it + 2) == 'e') {
                // we found "re" after the apostrophe, this is an "are" contraction
                string tempStr{tempValue.substr(0, foundIdx)};
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "are" contraction string
                contractionStr2 = "are";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            } else if (*(it + 1) == 'd') {
                // we found "d" after the apostrophe, this is a "did" contraction
                string tempStr{tempValue.substr(0, foundIdx)};
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "did" contraction string
                contractionStr2 = "did";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 2);
            } else if (*(it + 1) == 'm') {
                // we found "m" after the apostrophe, this is the "I'm" contraction
                string tempStr{tempValue.substr(0, foundIdx)};
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "am" contraction string
                contractionStr2 = "am";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 2);
            } else if (*(it + 1) == 'l' && *(it + 2) == 'l') {
                // we found "ll" after the apostrophe, this is a "will" contraction
                string tempStr{tempValue.substr(0, foundIdx)};
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "will" contraction string
                contractionStr2 = "will";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            } else if (*(it + 1) == 'v' && *(it + 2) == 'e') {
                // we found "ve" after the apostrophe, this is a "have" contraction
                string tempStr{tempValue.substr(0, foundIdx)};
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "have" contraction string
                contractionStr2 = "have";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            } else if (*(it - 1) == 'n' && *(it + 1) == 't') {
                // we found "n't", this is a "not" contraction, check up to foundIdx - 1 so the compare string doesn't have the n
                string tempStr{tempValue.substr(0, foundIdx - 1)};
                string compareStr{};
                bool specialContraction{false};
                // create the lowercase comparison string
                for (char c : tempStr) {
                    compareStr += static_cast<char>(tolower(c));
                }
                // compare to n't contractions that aren't just the word in front of the n't
                if (compareStr == "ai") {
                    // this contraction ain't is am not
                    contractionStr1 = "am";
                    specialContraction = true;
                } else if (compareStr == "wo") {
                    // this contraction won't is will not
                    contractionStr1 = "will";
                    specialContraction = true;
                } else if (compareStr == "ca") {
                    // this contraction can't is can not
                    contractionStr1 = "can";
                    specialContraction = true;
                }
                if (!specialContraction) {
                    // this isn't a special contraction, so the first word stays as is
                    contractionStr1 = compareStr;
                }
                // create the "not" contraction string
                contractionStr2 = "not";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 2);
            } else {
                // this didn't fit any contraction logic, so it's a weird word with an apostrophe
                // find the next punctuation or space so we keep the apostrophe
                it = find_if(it + 1, tempValue.end(), isPunctOrSpace);
                // update found index so we keep the rest of the word
                foundIdx = distance(tempValue.begin(), it);
            }
        }
        string subStr{};
        if (foundIdx == 0) {
            // punctuation or space found at index 0, erase it and continue
            tempValue.erase(0, 1);
        } else if (contractionStr1.size() > 0 && contractionStr2.size() > 0) {
            // we have contraction strings, add the tuples to the output buffer
            outputBuffer += "(";
            outputBuffer += contractionStr1;
            outputBuffer += ", 1)";
            outputBuffer += "(";
            outputBuffer += contractionStr2;
            outputBuffer += ", 1)";
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
    // ensure we export any remaining output if we finish parsing the data and the output buffer isn't empty
    if (outputBuffer.size() > 0) {
        exportData(aKey, outputBuffer);
        outputBuffer.clear();
    }
}

// exports tokenized values to the disk
void Map::exportData(const string& aKey, const string& aValue) {
    // TODO export the data with FileManagement
    // Initialize the output file stream
    std::ofstream outfile("out.txt", std::iostream::app);
    if (!outfile.is_open()) {
        std::cerr << "Error: could not open output file" << std::endl;
        return;
    }

    outfile << aValue;

    // Close the output file stream
    outfile.close();
}

#ifdef TEST_STUB
int main() {
    Map map;
    map.map("file", "Checking weird words like o'er and 't");
    // map.map("file", "you're we're they're who're what're when're where're why're how're which're");
    // Map mapguy;
    // string testStr = "This here is a string's string, go ahead and buffer this. You're probably going to see who's got apostrophes";
    // string fileName = "file";
    // mapguy.map(fileName, testStr);
    // string allswell = "ACT I\n";
    // allswell += "SCENE I. Rousillon. The COUNT's palace.\n\n";
    // allswell += "Enter BERTRAM, the COUNTESS of Rousillon, HELENA, and LAFEU, all in black\n";
    // allswell += "COUNTESS\n";
    // allswell += "In delivering my son from me, I bury a second husband.\n";
    // allswell += "BERTRAM\n";
    // allswell += "And I in going, madam, weep o'er my father's death\n";
    // allswell += "anew: but I must attend his majesty's command, to\n";
    // allswell += "whom I am now in ward, evermore in subjection.\n";
    // allswell += "LAFEU\n";
    // allswell += "You shall find of the king a husband, madam; you,\n";
    // allswell += "sir, a father: he that so generally is at all times\n";
    // allswell += "good must of necessity hold his virtue to you; whose\n";
    // allswell += "worthiness would stir it up where it wanted rather\n";
    // allswell += "than lack it where there is such abundance.\n";
    // allswell += "COUNTESS\n";
    // allswell += "What hope is there of his majesty's amendment?\n";
    // allswell += "LAFEU\n";
    // allswell += "He hath abandoned his physicians, madam; under whose\n";
    // allswell += "practises he hath persecuted time with hope, and\n";
    // allswell += "finds no other advantage in the process but only the\n";
    // allswell += "losing of hope by time.\n";
    // allswell += "COUNTESS\n";
    // allswell += "This young gentlewoman had a father,--O, that\n";
    // allswell += "'had'! how sad a passage 'tis!--whose skill was\n";
    // allswell += "almost as great as his honesty; had it stretched so\n";
    // allswell += "far, would have made nature immortal, and death\n";
    // allswell += "should have play for lack of work. Would, for the\n";
    // allswell += "king's sake, he were living! I think it would be\n";
    // allswell += "the death of the king's disease.\n";
    // allswell += "LAFEU\n";
    // allswell += "How called you the man you speak of, madam?\n";
    // allswell += "COUNTESS\n";
    // allswell += "He was famous, sir, in his profession, and it was\n";
    // allswell += "his great right to be so: Gerard de Narbon.\n";

    // fileName = "all's well that end's well";
    // mapguy.map(fileName, allswell);
}
#endif