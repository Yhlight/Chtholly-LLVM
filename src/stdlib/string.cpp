#include "string.hpp"

namespace chtholly::stdlib {

std::string get_string_module() {
    return R"(
#include <string>

namespace string {
    std::string concat(const std::string& s1, const std::string& s2) {
        return s1 + s2;
    }

    int len(const std::string& s) {
        return s.length();
    }

    std::string substr(const std::string& s, int start, int length) {
        return s.substr(start, length);
    }
}
)";
}

} // namespace chtholly::stdlib
