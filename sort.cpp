#include "sort.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

std::string Sort::extractString(const std::string& pairString) {
    size_t startIndex = pairString.find_first_of("(") + 1;
    size_t endIndex = pairString.find_last_of(",");
    return pairString.substr(startIndex, endIndex - startIndex);
}

void Sort::Reduce() {
    // Initialize the output file stream
    std::ofstream outfile("out.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error: could not open output file" << std::endl;
        return;
    }

    // Create a map to store the word counts
    std::map<std::string, std::vector<int>> word_counts;

    // Read from input.txt and update the counts
    std::ifstream infile("input.txt");
    if (infile.is_open()) {
        std::string word;
        while (getline(infile, word, ')')) {
            word = extractString(word);
            word_counts[word].push_back(1);
        }
        infile.close();
    }
    else {
        std::cerr << "Error: could not open input file" << std::endl;
        return;
    }

    // Sort the counts and write to out.txt
    std::vector<std::pair<std::string, std::vector<int>>> sorted_pairs(word_counts.begin(), word_counts.end());
    std::sort(sorted_pairs.begin(), sorted_pairs.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second.size() > rhs.second.size();
        });
    for (const auto& pair : sorted_pairs) {
        if (pair.first != " " && !pair.first.empty()) {
            outfile << "(" << pair.first << ", [";
            std::vector<int> counts = pair.second;
            std::sort(counts.begin(), counts.end());
            for (size_t i = 0; i < counts.size(); i++) {
                outfile << counts[i];
                if (i != counts.size() - 1) {
                    outfile << ", ";
                }
            }
            outfile << "]),";
        }
    }

    // Close the output file stream
    outfile.close();

    std::cout << "Finished writing to out.txt" << std::endl;
}