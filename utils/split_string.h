#pragma once
#include "vector.h"
#include <sstream>

namespace utils {
inline utils::vector<std::string> split_string (std::string str, char sep) {
    std::string s;
    std::stringstream ss (str);

    utils::vector<std::string> v;

    while (getline (ss, s, sep)) {
        v.push_back (s);
    }

    return v;
}
} // namespace utils
