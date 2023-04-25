#ifndef SORT_H
#define SORT_H

#include <map>
#include <string>
#include <vector>
#include <fstream>  // For std::ofstream

class Sort {
public:
    // Constructor that takes the input and output file names as arguments
    Sort(const std::string& input_filename, const std::string& output_filename);

    // Public function that reduces the input file and writes the output to the output file
    void Sorter();

private:
    std::string input_filename_;  // Input file name
    std::string output_filename_;  // Output file name
    std::string data;  // Input data
    std::string result;  // Output result

    // Private function that extracts the string between double quotes from a pair string
    std::string ExtractString(const std::string& pair_string);

    // Private function that updates the word counts map based on the input file
    bool UpdateWordCounts(std::map<std::string, std::vector<int>>& word_counts);

    // Private function that sorts the word counts map and writes the output to the output file
    void SortCountsToFile(const std::map<std::string, std::vector<int>>& word_counts);
};

#endif
