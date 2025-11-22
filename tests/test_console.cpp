#include "gtest/gtest.h"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/ASTPrinter.hpp"
#include "../src/Transpiler.hpp"
#include "test_helper.hpp"

TEST(ConsoleTest, SetColor) {
    std::string source = R"(
        import console;

        fn main() {
            console::set_color(console::ConsoleColor::Red);
        }
    )";

    chtholly::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    chtholly::Parser parser(tokens);
    auto stmts = parser.parse();

    chtholly::Transpiler transpiler("");
    std::string result = transpiler.transpile(stmts);

    std::string expected = R"(
        #include <string>
        #include <vector>
        #include <iostream>

        namespace console {
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
        }

        int main(int argc, char* argv[]) {
            console::set_color(console::ConsoleColor::Red);
        }
    )";
    ASSERT_EQ(normalize(result), normalize(expected));
}
