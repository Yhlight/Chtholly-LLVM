#ifndef CHTHOLLY_PARSER_H
#define CHTHOLLY_PARSER_H

#include "../lexer/Lexer.h"
#include "../ast/AST.h"
#include <vector>
#include <string>

namespace Chtholly
{
    class Parser
    {
    public:
        Parser(Lexer& lexer);

        std::unique_ptr<Program> ParseProgram();

    private:
        void NextToken();
        bool CurrentTokenIs(TokenType type);
        bool PeekTokenIs(TokenType type);
        bool ExpectPeek(TokenType type);

        std::unique_ptr<Statement> ParseStatement();
        std::unique_ptr<LetStatement> ParseLetStatement();
        std::unique_ptr<ReturnStatement> ParseReturnStatement();

        Lexer& m_lexer;
        Token m_currentToken;
        Token m_peekToken;
        std::vector<std::string> m_errors;
    };

} // namespace Chtholly

#endif // CHTHOLLY_PARSER_H
