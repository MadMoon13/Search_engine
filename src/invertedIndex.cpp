#include "invertedIndex.h"

// mutex for locking access to dictionary, so different threads will not get access to it simultaneously.
std::mutex dict_access;

// used to break down full string to a single words, which then will either be added to a dictionary, or
// will be used to increase counter on certain Entry object with matching id.
void InvertedIndex::breakDownString(const std::string &input, const int &id) {
    std::stringstream s;
    std::string temp;
    s << input;
    while (s >> temp) {
        // check if dictionary already contain current word
        if (!freq_dictionary.contains(temp)) {
            // if not - create new Entry, with count equal to 1, and doc_id equal to current id.
            Entry entry;
            entry.doc_id = id;
            entry.count = 1;
            std::vector<Entry> entries;
            entries.push_back(entry);
            dict_access.lock();
            freq_dictionary.insert(std::make_pair(temp, entries));
            dict_access.unlock();
        } else {
            // if word is already in a dictionary - check if existing Entry have a matching id. If yes - increase count.
            // if not - create new Entry.
            auto it = freq_dictionary.find(temp);
            bool match = false;
            for (auto &x : it->second) {
                if (id == x.doc_id) {
                    dict_access.lock();
                    x.count++;
                    dict_access.unlock();
                    match = true;
                    break;
                }
            }
            if (!match) {
                Entry entry;
                entry.doc_id = id;
                entry.count = 1;
                dict_access.lock();
                it->second.push_back(entry);
                dict_access.unlock();
            }
        }
    }
}

// main method of this class, used to form dictionary.
void InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    // use loop to start implementing previous method in a different threads, number of threads are equal to number of
    // elements in a passed vector.
    std::vector<std::thread> th;
    for (int i = 0; i < input_docs.size(); i++) {
        th.emplace_back(&InvertedIndex::breakDownString, this, input_docs[i], i);
    }
    // join all threads before finishing the job.
    for (auto &x : th) {
        x.join();
    }
}

// after dictionary is formed. current method allows us to get entries for a certain word.
std::vector<Entry> InvertedIndex::getWordsCount(const std::string &word) {
    if (freq_dictionary.contains(word)) {
        auto it = freq_dictionary.find(word);
        return it->second;
    } else {
        std::vector<Entry> empty;
        return empty;
    }
}

// supplementary method, if we want to output dictionary in the console.
void InvertedIndex::print() {
    for (auto &x : freq_dictionary) {
        std::cout << x.first << ": ";
        for (auto &y : x.second) {
            std::cout << "{" << y.doc_id << ", " << y.count << "}";
            if (&y == &*x.second.rbegin()) {
                std::cout << ";";
            } else {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}