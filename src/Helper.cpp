//
// Created by morbyosef on 1/9/19.
//

#include "Helper.h"

std::vector<std::string> Helper::splitString(const std::string &line, char delimiter){
    std::string word;
    std::vector<std::string> words;
    std::stringstream lineStream(line);

    while(std::getline(lineStream, word, delimiter)){
        words.push_back(word);
    }

    return words;
}

bool Helper::check_prefix(std::string &line, const std::string &of) {
    if (line.size() > of.size()) return false;
    return std::equal(line.begin(), line.end(), of.begin());
}