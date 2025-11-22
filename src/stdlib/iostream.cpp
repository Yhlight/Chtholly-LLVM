#include "iostream.hpp"

namespace chtholly::stdlib {

StdlibModule get_iostream_module() {
    return {
        R"(
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
)",
        {"iostream"}
    };
}

} // namespace chtholly::stdlib
