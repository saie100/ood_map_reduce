#ifndef SORT_H
#define SORT_H

#include <map>
#include <string>
#include <vector>

class Sort {
public:
    void Reduce(const std::string& input_filename, const std::string& output_filename);

private:
    std::string ExtractString(const std::string& pair_string);
    bool UpdateWordCounts(const std::string& input_filename, std::map<std::string, std::vector<int>>& word_counts);
    void SortCountsToFile(const std::map<std::string, std::vector<int>>& word_counts, std::ofstream& outfile);
};

#endif
