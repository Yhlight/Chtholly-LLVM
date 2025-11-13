#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer/Lexer.h"
#include "lexer/Token.h"

void PrintToken(const Chtholly::Token& token)
{
    std::cout << "Line: " << token.line << ", Column: " << token.column
              << ", Type: " << static_cast<int>(token.type)
              << ", Literal: " << token.literal << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Chtholly::Lexer lexer(source);

    Chtholly::Token token;
    do
    {
        token = lexer.NextToken();
        PrintToken(token);
    } while (token.type != Chtholly::TokenType::Eof);

    return 0;
}
