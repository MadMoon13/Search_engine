#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>

// every word in dictionary are related to this structure, which shows amount of times word was used in a document with
// certain id number.
struct Entry {
    size_t doc_id, count;
    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    std::vector<Entry> getWordsCount(const std::string& word);
    void breakDownString(const std::string& input, const int& id);
    void updateDocumentBase(const std::vector<std::string>& input_docs);
    void print();
private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};