#ifndef CHTHOLLY_STDLIB_CONSOLE_HPP
#define CHTHOLLY_STDLIB_CONSOLE_HPP

#include <string>
#include "../Stdlib.hpp"

namespace chtholly {
namespace stdlib {

const std::string console_module = R"(
enum ConsoleColor {
    Red,
    Green,
    Blue,
    Yellow,
    White,
    Black
};

fn set_color(color: ConsoleColor) {}
fn reset_color() {}
fn clear() {}
)";

StdlibModule get_console_module();

} // namespace stdlib
} // namespace chtholly

#endif // CHTHOLLY_STDLIB_CONSOLE_HPP
