#ifndef CHTHOLLY_CHTHOLLY_HPP
#define CHTHOLLY_CHTHOLLY_HPP

#include <string>

namespace chtholly {

class Chtholly {
public:
    void runFile(const std::string& path);
    void runPrompt();

private:
    void run(const std::string& source);
    void report(int line, const std::string& where, const std::string& message);

    bool hadError = false;
};

} // namespace chtholly

#endif // CHTHOLLY_CHTHOLLY_HPP
