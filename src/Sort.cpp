#include "headers/Sort.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

void Sort::Reduce(const string& input_filename, const string& output_filename) {
  ofstream outfile(output_filename);
  if (!outfile.is_open()) {
    cerr << "Error: could not open output file" << endl;
    return;
  }

  map<string, vector<int>> word_counts;

  if (!UpdateWordCounts(input_filename, word_counts)) {
    cerr << "Error: could not open input file" << endl;
    return;
  }

  SortCountsToFile(word_counts, outfile);

  outfile.close();

  cout << "Finished writing to " << output_filename << endl;
}

string Sort::ExtractString(const string& pair_string) {
  size_t start_index = pair_string.find_first_of("\"") + 1;
  size_t end_index = pair_string.find_last_of("\"");
  return pair_string.substr(start_index, end_index - start_index);
}

bool Sort::UpdateWordCounts(const string& input_filename,
                            map<string, vector<int>>& word_counts) {
  ifstream infile(input_filename);
  if (infile.is_open()) {
    string word;
    while (getline(infile, word, ')')) {
      word = ExtractString(word);
      word_counts[word].push_back(1);
    }
    infile.close();
    return true;
  } else {
    return false;
  }
}

void Sort::SortCountsToFile(const map<string, vector<int>>& word_counts,
                            ofstream& outfile) {
  vector<pair<string, vector<int>>> sorted_pairs(word_counts.begin(),
                                                 word_counts.end());
  sort(sorted_pairs.begin(), sorted_pairs.end(),
       [](const auto& lhs, const auto& rhs) {
         return lhs.second.size() > rhs.second.size();
       });
  for (const auto& pair : sorted_pairs) {
    if (pair.first != " " && !pair.first.empty()) {
      outfile << "(\"" << pair.first << "\", [";
      vector<int> counts = pair.second;
      sort(counts.begin(), counts.end());
      for (size_t i = 0; i < counts.size(); i++) {
        outfile << counts[i];
        if (i != counts.size() - 1) {
          outfile << ", ";
        }
      }
      outfile << "]),";
    }
  }
}
