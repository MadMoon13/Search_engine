#include <gtest/gtest.h>
#include "invertedIndex.h"

void test_inverted_index_functionality(
        const std::vector<std::string>& docs,
        const std::vector<std::string>& requests,
        const std::vector<std::vector<Entry>>& expected
        ) {
    InvertedIndex idx;
    std::vector<std::vector<Entry>> result;
    idx.updateDocumentBase(docs);
    for (auto &x : requests) {
        std::vector<Entry> entries = idx.getWordsCount(x);
        result.push_back(entries);
    }
    ASSERT_EQ(result, expected);
}

TEST(test_case_invertedIndex, test_basic) {
    std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the name of the Great bell of the striking clock"
    };
    std::vector<std::string> requests = {"london", "the"};
    std::vector<std::vector<Entry>> expected = {
            {
            {0, 1}},
            {{0, 1}, {1, 3}
    }
};
    test_inverted_index_functionality(docs, requests, expected);
}

TEST(test_case_invertedIndex, test_basic_second) {
    std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    std::vector<std::string> requests = {
            "milk", "water", "cappuccino"
    };
    std::vector<std::vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            },
            {
                    {0, 3}, {1, 2}, {2, 5}
            },
            {
                    {3, 1}
            }
    };
    test_inverted_index_functionality(docs, requests, expected);
}

TEST(test_case_invertedIndex, test_missing_word) {
    std::vector<std::string> docs = {"a b c d e f g h i j k l", "statement"};
    std::vector<std::string> requests = {"m", "statement"};
    std::vector<std::vector<Entry>> expected = {
            {

            },
            {
                {1, 1}
            }
    };
    test_inverted_index_functionality(docs, requests, expected);
}