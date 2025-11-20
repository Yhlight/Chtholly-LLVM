#ifndef CHTHOLLY_CHTHOLLY_HPP
#define CHTHOLLY_CHTHOLLY_HPP

#include "Lexer.hpp"
#include <string>

namespace chtholly {

class Chtholly {
public:
    void runFile(const std::string& path);
    void runPrompt();

    static void error(int line, const std::string& message);
    static void error(Token token, const std::string& message);
    static void report(int line, const std::string& where, const std::string& message);

private:
    void run(const std::string& source);

    static bool hadError;
};

} // namespace chtholly

#endif // CHTHOLLY_CHTHOLLY_HPP
