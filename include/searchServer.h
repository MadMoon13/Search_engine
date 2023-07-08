#pragma once

#include <unordered_set>
#include "invertedIndex.h"

// this structure reflects overall relevance of document with certain id to a request.
struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex& idx) : _index(idx) {}
    void setMaxAnswers(int _max);
    std::vector<std::vector<Entry>> getEntriesForString(const std::string& string);
    std::vector<RelativeIndex> getIndexForString(const std::string& string);
    float getMaxRank(const std::vector<RelativeIndex>& indexes);
    void formRelativeRanks(std::vector<RelativeIndex>& indexes);
    void sortByMaxRank(std::vector<RelativeIndex>& indexes);
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
    std::vector<std::vector<std::pair<int, float>>> convertToPair(const std::vector<std::vector<RelativeIndex>>& indexes);
private:
    int max_answers = 5;
    InvertedIndex _index;
};