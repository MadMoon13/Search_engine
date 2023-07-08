#include "searchServer.h"

// this method defines number of answers relevant to a request to be formed in to answers.json.
void SearchServer::setMaxAnswers(int _max) {
    _max > max_answers ? max_answers = _max : max_answers;
}

// after receiving string from a request, break it down to individual words, filter them to form a list of unique ones.
// After that get relevant entries for them from a dictionary.
 std::vector<std::vector<Entry>> SearchServer::getEntriesForString(const std::string &string) {
    std::vector<std::vector<Entry>> result;
    std::unordered_set<std::string> list;
    std::stringstream s;
    std::string temp;
    s << string;
    while (s >> temp) {
        if (!list.contains(temp)) {
            list.insert(temp);
        }
    }
    for (auto &x : list) {
        auto entries = _index.getWordsCount(x);
        result.push_back(entries);
    }
    return result;
}

// implement previous method to get entries, and form a vector with RelativeIndex objects type instead of Entry.
std::vector<RelativeIndex> SearchServer::getIndexForString(const std::string &string) {
     std::vector<RelativeIndex> result;
     std::unordered_set<size_t> filter;
     auto entries = getEntriesForString(string);
     for (auto &x : entries) {
         for (auto &y : x) {
             RelativeIndex index;
             index.doc_id = y.doc_id;
             index.rank = 0;
             if (!filter.contains(index.doc_id)) {
                 for (auto &z: entries) {
                     for (auto &q: z) {
                         if (q.doc_id == index.doc_id) {
                             index.rank += q.count;
                         }
                     }
                 }
                 filter.insert(index.doc_id);
                 result.push_back(index);
             }
         }
     }
    return result;
 }

// get maximum rank, i.e. number of times document with certain id was relevant to a request.
float SearchServer::getMaxRank(const std::vector<RelativeIndex> &indexes) {
    float max = 0;
    for (auto &x : indexes) {
        x.rank > max ? max = x.rank : max;
    }
    return max;
}

// find max rank and form ranks in relation to it.
void SearchServer::formRelativeRanks(std::vector<RelativeIndex> &indexes) {
    auto max = getMaxRank(indexes);
    if (max > 0) {
        for (auto &x : indexes) {
            x.rank /= max;
        }
    }
}

// sort our results by two parameters: 1 - highest rank, 2 - lowest doc_id.
void SearchServer::sortByMaxRank(std::vector<RelativeIndex> &indexes) {
    for (auto &x : indexes) {
        for (auto &y : indexes) {
            if (x.rank > y.rank) {
                std::swap(x, y);
            } else if (x.rank == y.rank && x.doc_id < y.doc_id) {
                std::swap(x, y);
            }
        }
    }
}

// finally, after passing requests to this method, we form a vector, containing RelativeIndex objects for every string
// of a request.
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
     std::vector<std::vector<RelativeIndex>> result;
        for (auto &x: queries_input) {
            auto indexes = getIndexForString(x);
            if (indexes.empty()) {
                std::vector<RelativeIndex> empty;
                result.push_back(empty);
            } else {
                sortByMaxRank(indexes);
                formRelativeRanks(indexes);
                if (indexes.size() < max_answers) {
                    result.push_back(indexes);
                } else {
                    std::vector<RelativeIndex> shortened_indexes;
                    for (auto &y: indexes) {
                        shortened_indexes.push_back(y);
                        if (shortened_indexes.size() >= max_answers) {
                            break;
                        }
                    }
                    result.push_back(shortened_indexes);
                }
            }
        }
     return result;
 }

// it was specified in the task, what we pass a vector, containing pairs to ConverterJSON method to form answers.json.
// Current method create an object of this type, based on a results of "search" method.
std::vector<std::vector<std::pair<int, float>>>
SearchServer::convertToPair(const std::vector<std::vector<RelativeIndex>> &indexes) {
    std::vector<std::vector<std::pair<int, float>>> conversion_result;
    for (auto &x : indexes) {
        if (!x.empty()) {
            std::vector<std::pair<int, float>> pairs;
            for (auto &y : x) {
                std::pair<int, float> pair;
                pair.first = y.doc_id;
                pair.second = y.rank;
                pairs.push_back(pair);
            }
            conversion_result.push_back(pairs);
        } else {
            std::vector<std::pair<int, float>> empty;
            conversion_result.push_back(empty);
        }
    }
    return conversion_result;
}