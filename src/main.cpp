#include <iostream>
#include <vector>
#include "Lexer.hpp"
#include "Token.hpp"

// Helper function to convert TokenType to string for printing
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LET: return "LET";
        case TokenType::MUT: return "MUT";
        case TokenType::FUNC: return "FUNC";
        case TokenType::RETURN: return "RETURN";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::STRUCT: return "STRUCT";
        case TokenType::ENUM: return "ENUM";
        case TokenType::PUBLIC: return "PUBLIC";
        case TokenType::PRIVATE: return "PRIVATE";
        case TokenType::IMPL: return "IMPL";
        case TokenType::IMPORT: return "IMPORT";
        case TokenType::SWITCH: return "SWITCH";
        case TokenType::CASE: return "CASE";
        case TokenType::FALLTHROUGH: return "FALLTHROUGH";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NONE: return "NONE";
        case TokenType::SELF: return "SELF";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::DOUBLE: return "DOUBLE";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::PERCENT: return "PERCENT";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::LOGICAL_AND: return "LOGICAL_AND";
        case TokenType::LOGICAL_OR: return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT: return "LOGICAL_NOT";
        case TokenType::BITWISE_AND: return "BITWISE_AND";
        case TokenType::BITWISE_OR: return "BITWISE_OR";
        case TokenType::BITWISE_XOR: return "BITWISE_XOR";
        case TokenType::BITWISE_NOT: return "BITWISE_NOT";
        case TokenType::LEFT_SHIFT: return "LEFT_SHIFT";
        case TokenType::RIGHT_SHIFT: return "RIGHT_SHIFT";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TokenType::STAR_ASSIGN: return "STAR_ASSIGN";
        case TokenType::SLASH_ASSIGN: return "SLASH_ASSIGN";
        case TokenType::PERCENT_ASSIGN: return "PERCENT_ASSIGN";
        case TokenType::ARROW: return "ARROW";
        case TokenType::COLON_COLON: return "COLON_COLON";
        case TokenType::DOT: return "DOT";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN_DEFAULT";
    }
}


int main(int argc, char* argv[]) {
    std::string source = R"(
        // This is a test program
        func main() -> int {
            let x = 10;
            mut y = "hello";
            if (x >= 10) {
                print(y);
            }
            return 0;
        }
    )";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Type: " << tokenTypeToString(token.type)
                  << ", Lexeme: '" << token.lexeme
                  << "', Line: " << token.line << std::endl;
    }

    return 0;
}
