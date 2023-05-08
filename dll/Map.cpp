#include "../src/headers/FileManager.h"
#include <cctype>
#include <algorithm>
#include <array>
#include <iostream>

using std::find_if;
using std::distance;

// checks if the passed in character is a punctuation or whitespace character
bool isPunctOrSpace(int aChar) {
    return (ispunct(aChar) || isspace(aChar));
}

// exports tokenized values to the disk
void exportData(const string& aKey, const string& aValue, const string& aOutputPath) {
    // write the data to the file
    FileManager::writeFile(FileManager::MODE::APPEND, aOutputPath, aValue);
}

// tokenizes the value into distinct words, removing whitespace, punctuation, and capitalization
extern "C" void map(const string& aKey, const string& aValue, const string& aOutputPath) {
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
                string tempStr{ tempValue.substr(0, foundIdx) };
                string compareStr{};
                bool isContraction{ false };
                // create the lowercase comparison string
                for (char c : tempStr) {
                    compareStr += static_cast<char>(tolower(c));
                }
                // compare to common contractions for 's
                if (compareStr == "it" || compareStr == "he" || compareStr == "she" || compareStr == "who" || compareStr == "what"
                    || compareStr == "when" || compareStr == "where" || compareStr == "why" || compareStr == "how" || compareStr == "which"
                    || compareStr == "this" || compareStr == "that" || compareStr == "here" || compareStr == "there" || compareStr == "somebody"
                    || compareStr == "someone" || compareStr == "something") {
                    // this is a contraction, set up the strings
                    contractionStr1 = compareStr;
                    contractionStr2 = "is";
                    isContraction = true;
                }
                if (isContraction) {
                    // erase the contraction from tempValue
                    tempValue.erase(tempValue.begin(), it + 2);
                }
                else {
                    // erase the apostrophe and the s since this is a possessive
                    tempValue.erase(it, it + 2);
                }
            }
            else if (*(it + 1) == 'r' && *(it + 2) == 'e') {
                // we found "re" after the apostrophe, this is an "are" contraction
                string tempStr{ tempValue.substr(0, foundIdx) };
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "are" contraction string
                contractionStr2 = "are";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            }
            else if (*(it + 1) == 'd') {
                // we found "d" after the apostrophe, this is a "did" contraction
                string tempStr{ tempValue.substr(0, foundIdx) };
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "did" contraction string
                contractionStr2 = "did";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 2);
            }
            else if (*(it + 1) == 'm') {
                // we found "m" after the apostrophe, this is the "I'm" contraction
                string tempStr{ tempValue.substr(0, foundIdx) };
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "am" contraction string
                contractionStr2 = "am";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 2);
            }
            else if (*(it + 1) == 'l' && *(it + 2) == 'l') {
                // we found "ll" after the apostrophe, this is a "will" contraction
                string tempStr{ tempValue.substr(0, foundIdx) };
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "will" contraction string
                contractionStr2 = "will";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            }
            else if (*(it + 1) == 'v' && *(it + 2) == 'e') {
                // we found "ve" after the apostrophe, this is a "have" contraction
                string tempStr{ tempValue.substr(0, foundIdx) };
                // create the lowercase contraction string
                for (char c : tempStr) {
                    contractionStr1 += static_cast<char>(tolower(c));
                }
                // create the "have" contraction string
                contractionStr2 = "have";
                // erase the contraction from tempValue
                tempValue.erase(tempValue.begin(), it + 3);
            }
            else if (*(it - 1) == 'n' && *(it + 1) == 't') {
                // we found "n't", this is a "not" contraction, check up to foundIdx - 1 so the compare string doesn't have the n
                string tempStr{ tempValue.substr(0, foundIdx - 1) };
                string compareStr{};
                bool specialContraction{ false };
                // create the lowercase comparison string
                for (char c : tempStr) {
                    compareStr += static_cast<char>(tolower(c));
                }
                // compare to n't contractions that aren't just the word in front of the n't
                if (compareStr == "ai") {
                    // this contraction ain't is am not
                    contractionStr1 = "am";
                    specialContraction = true;
                }
                else if (compareStr == "wo") {
                    // this contraction won't is will not
                    contractionStr1 = "will";
                    specialContraction = true;
                }
                else if (compareStr == "ca") {
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
            }
            else {
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
        }
        else if (contractionStr1.size() > 0 && contractionStr2.size() > 0) {
            // we have contraction strings, add the tuples to the output buffer
            outputBuffer += "(\"";
            outputBuffer += contractionStr1;
            outputBuffer += "\",1)";
            outputBuffer += "(\"";
            outputBuffer += contractionStr2;
            outputBuffer += "\",1)";
        }
        else {
            // punctuation or space found, grab the string
            subStr = tempValue.substr(0, foundIdx);
            // add the (word, 1) pair to the output buffer
            outputBuffer += "(\"";
            // loop through the sub string, adding the word as a lowercase string to the output buffer
            for (char c : subStr) {
                outputBuffer += static_cast<char>(tolower(c));
            }
            outputBuffer += "\",1)";
            // erase the word from tempValue
            tempValue.erase(tempValue.begin(), it);
            if (tempValue.size() > 0) {
                // erase the whitespace or punctuation, and trailing s if necessary, then continue
                tempValue.erase(0, 1);
            }
        }

        // if the output buffer has over 100 characters in it, export the data to the temporary file
        if (outputBuffer.size() > 100) {
            exportData(aKey, outputBuffer, aOutputPath);
            outputBuffer.clear();
        }
    }
    // ensure we export any remaining output if we finish parsing the data and the output buffer isn't empty
    if (outputBuffer.size() > 0) {
        exportData(aKey, outputBuffer, aOutputPath);
        outputBuffer.clear();
    }
}