#pragma once
#include <vector>
#include <sstream>
#include <string>

namespace Util {

std::vector<std::string> splitString(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim)) {
        res.push_back(token);
    }

    return res;
}

}
