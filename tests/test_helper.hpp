#ifndef CHTHOLLY_TEST_HELPER_HPP
#define CHTHOLLY_TEST_HELPER_HPP

#include <string>
#include <algorithm>
#include <cctype>

// Function to remove all whitespace from a string
inline std::string normalize(const std::string& s) {
    std::string result;
    std::remove_copy_if(s.begin(), s.end(), std::back_inserter(result), [](char c) {
        return std::isspace(static_cast<unsigned char>(c));
    });
    return result;
}

#endif // CHTHOLLY_TEST_HELPER_HPP
