#ifndef SORT_H
#define SORT_H

#include <map>
#include <string>
#include <vector>
#include <fstream>  // For std::ofstream

using std::string;

class Sort {
public:
    // Constructor that takes the input and output file names as arguments
    Sort(const string& input_filename, const string& output_filename);

    // Public function that reduces the input file and writes the output to the output file
    void Sorter();

private:
    string input_filename_;  // Input file name
    string output_filename_;  // Output file name
    string data;  // Input data
    string result;  // Output result

    // Private function that extracts the string between double quotes from a pair string
    string ExtractString(const string& pair_string);

    // Private function that updates the word counts map based on the input file
    bool UpdateWordCounts(std::map<string, std::vector<int>>& word_counts);

    // Private function that sorts the word counts map and writes the output to the output file
    void SortCountsToFile(const std::map<string, std::vector<int>>& word_counts, string outputFilePath);
};

#endif
