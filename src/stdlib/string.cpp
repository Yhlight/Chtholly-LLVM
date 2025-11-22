#include "string.hpp"

namespace chtholly::stdlib {

StdlibModule get_string_module() {
    return {
        R"(
    std::string concat(const std::string& s1, const std::string& s2) {
        return s1 + s2;
    }

    int len(const std::string& s) {
        return s.length();
    }

    std::string substr(const std::string& s, int start, int length) {
        return s.substr(start, length);
    }

    std::string to_upper(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    std::string to_lower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            return "";
        }
        auto end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
        if (delimiter.empty()) {
            throw std::invalid_argument("delimiter cannot be empty");
        }
        std::vector<std::string> tokens;
        std::string::size_type start = 0;
        std::string::size_type end = s.find(delimiter);
        while (end != std::string::npos) {
            tokens.push_back(s.substr(start, end - start));
            start = end + delimiter.length();
            end = s.find(delimiter, start);
        }
        tokens.push_back(s.substr(start));
        return tokens;
    }

    std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i < parts.size() - 1) {
                result += delimiter;
            }
        }
        return result;
    }
)",
        {"string", "vector", "algorithm"}
    };
}

} // namespace chtholly::stdlib
