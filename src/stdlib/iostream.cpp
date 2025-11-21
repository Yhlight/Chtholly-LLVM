#include "iostream.hpp"

namespace chtholly::stdlib {

std::string get_iostream_module() {
    return R"(
namespace iostream {
    template<typename T>
    void print(const T& msg) {
        std::cout << msg;
    }

    template<typename T>
    void println(const T& msg) {
        std::cout << msg << std::endl;
    }
}
)";
}

} // namespace chtholly::stdlib
