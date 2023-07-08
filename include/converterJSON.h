#pragma once
#include <vector>
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>
#include "nlohmann/json.hpp"

// exception for a case of non-existing config
class ConfigNotFound: public std::exception {
public:
    const char* what() const noexcept override {
        return "Can not open file \"config.json\". Please make sure what file is placed in the project root directory\n";
    }
};

// exception for a case of missing source files
class MissingFiles: public std::exception {
public:
    const char* what() const noexcept override {
        return "One or more files, defined as source files by \"config.json\" are missing.\n"
               "Please make sure all files are present in the project root directory.\n";
    }
};

// exception for a missing requests file. Since implementation was not specified in the task,
// if corresponding file is missing, it will terminate the execution of a program.
class MissingRequest: public std::exception {
    const char* what() const noexcept override {
        return "File \"requests.json\" is missing. Please make sure what file is placed in the project root directory.\n";
    }
};


class ConverterJSON {
public:
    ConverterJSON() = default;
    std::vector<std::string> getTextDocuments();
    std::vector<std::string> getRequests();
    int getResponsesLimit();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);
};