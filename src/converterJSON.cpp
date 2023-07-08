#include "converterJSON.h"

// this method implemented to get content out of the source files with .txt extension and convert them to strings.
std::vector<std::string> ConverterJSON::getTextDocuments() {
    std::vector<std::string> documents;
    std::string path_to_config = "..\\..\\config.json"; // relative path to config file.
    std::ifstream read_config(path_to_config);
    if (!read_config.is_open()) {
        // terminate execution if config is not found, have different extension, corrupted, etc.
        throw ConfigNotFound();
    }
    nlohmann::json from_file;
    read_config >> from_file;
    read_config.close();
    bool all_is_open = true; // check if all source files opened.
    for (auto it = from_file["files"].begin(); it != from_file["files"].end(); it++) {
        std::string path_to_file, file_content;
        path_to_file = it.value();
        std::ifstream content(path_to_file);
        if (content.is_open()) {
            std::getline(content, file_content);
        } else {
            all_is_open = false;
        }
        content.close();
        documents.push_back(file_content);
    }
    if (!all_is_open) {
        // if after iteration through files, defined as source files by config, some of them are missing or can not be
        // opened - there will be a warning, but execution won't be stopped.
        throw MissingFiles();
    }
    return documents;
}

// method to perform similar conversion for a requests file.
std::vector<std::string> ConverterJSON::getRequests() {
    std::vector<std::string> requests;
    std::string path_to_requests = "..\\..\\requests.json";
    std::ifstream read_requests(path_to_requests);
    nlohmann::json from_file;
    if (read_requests.is_open()) {
        read_requests >> from_file;
    } else {
        // will terminate execution in case of missing requests file.
        throw MissingRequest();
    }
    read_requests.close();
    for (auto it = from_file["requests"].begin(); it != from_file["requests"].end(); it++) {
        std::string content = it.value();
        requests.push_back(content);
    }
    return requests;
}

// method speaks for itself, we are getting maximum amount of responses.
int ConverterJSON::getResponsesLimit() {
    auto path_to_config = std::filesystem::relative("C:\\SB\\SE_5\\config.json");
    std::ifstream read_config(path_to_config);
    nlohmann::json from_file;
    read_config >> from_file;
    return from_file["config"].find("max_responses").value();
}

// after all of implemented classes finished their job, this method will form .json document with results.
void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    nlohmann::ordered_json put_answers;
    std::string request_base_name = "request";
    std::string request_number = "00";
    std::vector<std::string> requests_name;
    std::vector<bool> check_not_empty;
    for (int i = 0; i < answers.size(); i++) {
        bool check;
        if (answers[i].empty()) {
            check = false;
        } else {
            check = true;
        }
        check_not_empty.push_back(check);
        std::string finish_number = std::to_string(i + 1);
        std::string request = request_base_name + request_number + finish_number;
        requests_name.push_back(request);
    }
    std::vector<std::vector<nlohmann::json>> relevancy;
    for (auto &x : answers) {
        if (x.empty()) {
            std::vector<nlohmann::json> empty;
            relevancy.push_back(empty);
        } else {
            std::vector<nlohmann::json> relevant_pairs;
            for (auto &y : x) {
                nlohmann::json pair;
                pair = {{"docid", y.first}, {"rank", y.second}};
                relevant_pairs.push_back(pair);
            }
            relevancy.push_back(relevant_pairs);
        }
    }
    for (int i = 0; i < answers.size(); i++) {
        put_answers["answers:"][requests_name[i]]["result"] = {check_not_empty[i]};
        if (check_not_empty[i] == true) {
            if (relevancy.size() > 1) {
            put_answers["answers:"][requests_name[i]]["relevance"] = {relevancy[i]};
            } else {
                put_answers["answers:"][requests_name[i]] = {relevancy[i]};
            }
        }
    }
    std::ofstream file("..\\..\\answers.json");
    if (file.is_open()) {
        file.clear();
    }
    file << put_answers;
}