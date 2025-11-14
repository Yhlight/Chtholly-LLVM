#include "Parser.h"
#include <memory>

namespace Chtholly
{
    Parser::Parser(Lexer& lexer) : m_lexer(lexer)
    {
        NextToken();
        NextToken();
    }

    std::unique_ptr<Program> Parser::ParseProgram()
    {
        auto program = std::make_unique<Program>();
        while (m_currentToken.type != TokenType::Eof)
        {
            auto stmt = ParseStatement();
            if (stmt)
            {
                program->statements.push_back(std::move(stmt));
            }
            NextToken();
        }
        return program;
    }

    void Parser::NextToken()
    {
        m_currentToken = m_peekToken;
        m_peekToken = m_lexer.NextToken();
    }

    bool Parser::CurrentTokenIs(TokenType type)
    {
        return m_currentToken.type == type;
    }

    bool Parser::PeekTokenIs(TokenType type)
    {
        return m_peekToken.type == type;
    }

    bool Parser::ExpectPeek(TokenType type)
    {
        if (PeekTokenIs(type))
        {
            NextToken();
            return true;
        }
        else
        {
            // TODO: Add error handling
            return false;
        }
    }

    std::unique_ptr<Statement> Parser::ParseStatement()
    {
        switch (m_currentToken.type)
        {
        case TokenType::Let:
            return ParseLetStatement();
        case TokenType::Return:
            return ParseReturnStatement();
        default:
            return nullptr;
        }
    }

    std::unique_ptr<LetStatement> Parser::ParseLetStatement()
    {
        auto stmt = std::make_unique<LetStatement>(m_currentToken);

        if (!ExpectPeek(TokenType::Identifier))
        {
            return nullptr;
        }

        stmt->name = std::make_unique<Identifier>(m_currentToken, m_currentToken.literal);

        if (!ExpectPeek(TokenType::Assign))
        {
            return nullptr;
        }

        // TODO: Skipping expressions for now
        while (!CurrentTokenIs(TokenType::Semicolon))
        {
            NextToken();
        }

        return stmt;
    }

    std::unique_ptr<ReturnStatement> Parser::ParseReturnStatement()
    {
        auto stmt = std::make_unique<ReturnStatement>(m_currentToken);

        NextToken();

        // TODO: Skipping expressions for now
        while (!CurrentTokenIs(TokenType::Semicolon))
        {
            NextToken();
        }

        return stmt;
    }

} // namespace Chtholly
