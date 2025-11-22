#include "console.hpp"

namespace chtholly {
namespace stdlib {

const std::string console_source = R"(
enum class ConsoleColor {
    Red,
    Green,
    Blue,
    Yellow,
    White,
    Black
};

void set_color(ConsoleColor color) {
    switch (color) {
        case ConsoleColor::Red:
            std::cout << "\033[31m";
            break;
        case ConsoleColor::Green:
            std::cout << "\033[32m";
            break;
        case ConsoleColor::Blue:
            std::cout << "\033[34m";
            break;
        case ConsoleColor::Yellow:
            std::cout << "\033[33m";
            break;
        case ConsoleColor::White:
            std::cout << "\033[37m";
            break;
        case ConsoleColor::Black:
            std::cout << "\033[30m";
            break;
    }
}

void reset_color() {
    std::cout << "\033[0m";
}

void clear() {
    std::cout << "\033[2J\033[H";
}
)";

StdlibModule get_console_module() {
    return {console_source, {"iostream"}};
}

} // namespace stdlib
} // namespace chtholly
