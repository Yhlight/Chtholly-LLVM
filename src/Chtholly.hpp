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
};

} // namespace chtholly

#endif // CHTHOLLY_CHTHOLLY_HPP
