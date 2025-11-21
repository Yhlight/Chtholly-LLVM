#ifndef CHTHOLLY_TEST_HELPER_HPP
#define CHTHOLLY_TEST_HELPER_HPP

#include <string>
#include <algorithm>
#include <cctype>

// Helper function to normalize C++ code for comparison
inline std::string normalize(const std::string& s) {
    std::string result = s;
    // Remove all whitespace
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

#endif // CHTHOLLY_TEST_HELPER_HPP
