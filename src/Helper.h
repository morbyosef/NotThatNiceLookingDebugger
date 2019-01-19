//
// Created by morbyosef on 1/9/19.
//

#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

#include <sstream>
#include <vector>
#include <string>

class Helper {
public:
    static std::vector<std::string> splitString(const std::string &line, char delimiter);
    static bool check_prefix(std::string &line, const std::string &of);
};


#endif //UNTITLED_HELPER_H
