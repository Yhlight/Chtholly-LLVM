#ifndef CHTHOLLY_AST_H
#define CHTHOLLY_AST_H

#include <string>
#include <vector>
#include <memory>
#include "../lexer/Token.h"

namespace Chtholly
{
    // Base class for all nodes
    struct Node
    {
        virtual ~Node() = default;
        virtual std::string TokenLiteral() const = 0;
    };

    // Base class for all statements
    struct Statement : public Node
    {
        std::string TokenLiteral() const override { return "Statement"; }
    };

    // Base class for all expressions
    struct Expression : public Node
    {
        std::string TokenLiteral() const override { return "Expression"; }
    };

    // The root node of the AST
    struct Program : public Node
    {
        std::string TokenLiteral() const override { return "Program"; }
        std::vector<std::unique_ptr<Statement>> statements;
    };

    // Represents an identifier
    struct Identifier : public Expression
    {
        Identifier(Token token, const std::string& value) : token(token), value(value) {}
        std::string TokenLiteral() const override { return token.literal; }
        Token token;
        std::string value;
    };

    // Represents a let statement: let <name> = <value>;
    struct LetStatement : public Statement
    {
        LetStatement(Token token) : token(token) {}
        std::string TokenLiteral() const override { return token.literal; }
        Token token; // The 'let' token
        std::unique_ptr<Identifier> name;
        std::unique_ptr<Expression> value;
    };

    // Represents a return statement: return <value>;
    struct ReturnStatement : public Statement
    {
        ReturnStatement(Token token) : token(token) {}
        std::string TokenLiteral() const override { return token.literal; }
        Token token; // The 'return' token
        std::unique_ptr<Expression> returnValue;
    };

} // namespace Chtholly

#endif // CHTHOLLY_AST_H
