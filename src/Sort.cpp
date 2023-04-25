
#include "headers/Sort.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <iostream>
#include <sstream>
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::sort;
using std::string;
using std::vector;
// Constructor that sets the input and output file names
Sort::Sort(const std::string& input_filename, const std::string& output_filename,const std::string& outputPath)
    : input_filename_(input_filename), output_filename_(output_filename) {}


// The main function that sorts the word counts and writes the output to a file
void Sort::Sorter() {
    data = "(\"is\", 1), (\"the\", 1), (\"a\", 1), (\"is\", 1), (\"the\", 1), (\"a\", 1), (\"go\", 1), (\"play\", 1), (\"a\", 1), (\"is\", 1), (\"the\", 1), (\"a\", 1), (\"why\", 1), (\"go\", 1), (\"play\", 1), (\"a\", 1), (\"jump\", 1), (\"the\", 1), (\"a\", 1), (\"why\", 1), (\"go\", 1), (\"play\", 1), (\"a\", 1), (\"view\", 1), (\"the\", 1), (\"a\", 1)";
    /// calling FileManager::readFile(string input_filename);
   
    
    /// ////////////////////////////////////////////////////////////////////
   // array<string, 2> arr;
   // arr= FileManager::readFile(string input_filename);
   // data = arr[1];
    //////////////////////////////////////////////////////////////////////////////
   
   
   
    // Create a map to store word counts
    map<string, vector<int>> word_counts;

    // Update the word counts based on the input data
    if (!UpdateWordCounts(word_counts)) {
        cerr << "Error: could not open input file" << endl;
        return;
    }

    // Sort the word counts and write them to the output file
    SortCountsToFile(word_counts);


    ///////////////////////////////////////////////////////////////////////////////
    // print to output_filename 
    //FileManager::writeFile(CREATE, string outputPath, output_filename_, result)
    // 
    /////////////////////////////////////////////////////////////////////////////////



    // Print a message to indicate that the sorting is complete
    cout << "Finished writing to " << output_filename_ << endl;
}
// Helper function to extract a string from a pair string
string Sort::ExtractString(const string& pair_string) {


    // Find the start and end index of the string within the pair string
    size_t start_index = pair_string.find_first_of("\"") + 1;
    size_t end_index = pair_string.find_last_of("\"");

    // Find the start and end index of the string within the pair string
    return pair_string.substr(start_index, end_index - start_index);

}
// Helper function to update the word counts based on the input data
bool Sort::UpdateWordCounts(map<string, vector<int>>& word_counts) {
    
    if (!data.empty()) {
        // Remove numbers from input string
        data = std::regex_replace(data, std::regex("\\d+"), "");

        // Split input string into words
        std::vector<std::string> words;
        size_t start = data.find("(\"");
        while (start != std::string::npos) {
            size_t end = data.find(",", start + 2);
            if (end == std::string::npos) {
                end = data.find(")", start + 2);
            }
            std::string word = data.substr(start + 2, end - start - 3);
            if (!word.empty()) {
                words.push_back("\"" + ExtractString(word) + "\"");
            }
            start = data.find("(\"", end);
        }


        // Add words to word_counts map
        for (const std::string& word : words) {
            if (!word.empty()) {
                word_counts[word].push_back(1);
            }
        }

        return true;
    }
    else {
        return false;

    }
    infile.close();
    return true;
  } else {
    return false;
  }
}


void Sort::SortCountsToFile(const map<string, vector<int>>& word_counts) {
    string result; // A string to store the final result

    // Create a vector of pairs from the word_counts map and sort it by the size of the value vector in descending order
    vector<pair<string, vector<int>>> sorted_pairs(word_counts.begin(), word_counts.end());
    sort(sorted_pairs.begin(), sorted_pairs.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second.size() > rhs.second.size();
        });

    // Iterate over each pair in the sorted_pairs vector
    for (const auto& pair : sorted_pairs) {
        // Check if the key of the pair is not a space and not empty
        if (pair.first != " " && !pair.first.empty()) {
            result += "(" + pair.first + ", ["; // Add the key to the result string
            vector<int> counts = pair.second; // Get the value vector from the pair
            sort(counts.begin(), counts.end()); // Sort the value vector in ascending order

            // Iterate over each count in the value vector
            for (size_t i = 0; i < counts.size(); i++) {
                result += std::to_string(counts[i]); // Add the count to the result string
                if (i != counts.size() - 1) {
                    result += ", "; // Add a comma and space if it's not the last count
                }
            }

            result += "]),"; // Add closing brackets and comma to the result string

        }
      }
      outfile << "]),";
    }


    // Print the result string to the console
    std::cout << result << std::endl;

    

}
