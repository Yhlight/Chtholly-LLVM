#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "ast/AST.h"
#include <vector>
#include <string>

TEST(ParserTest, TestLetStatements)
{
    std::string input = R"(
        let x = 5;
        let y = 10;
        let foobar = 838383;
    )";

    Chtholly::Lexer lexer(input);
    Chtholly::Parser parser(lexer);
    auto program = parser.ParseProgram();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 3);

    std::vector<std::string> expected_identifiers = {"x", "y", "foobar"};

    for (size_t i = 0; i < expected_identifiers.size(); ++i)
    {
        auto stmt = program->statements[i].get();
        ASSERT_NE(stmt, nullptr);

        auto letStmt = dynamic_cast<Chtholly::LetStatement*>(stmt);
        ASSERT_NE(letStmt, nullptr);

        ASSERT_EQ(letStmt->token.type, Chtholly::TokenType::Let);
        ASSERT_EQ(letStmt->name->value, expected_identifiers[i]);
        ASSERT_EQ(letStmt->name->TokenLiteral(), expected_identifiers[i]);
    }
}

TEST(ParserTest, TestReturnStatements)
{
    std::string input = R"(
        return 5;
        return 10;
        return 993322;
    )";

    Chtholly::Lexer lexer(input);
    Chtholly::Parser parser(lexer);
    auto program = parser.ParseProgram();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 3);

    for (const auto& stmt_ptr : program->statements)
    {
        auto stmt = stmt_ptr.get();
        ASSERT_NE(stmt, nullptr);

        auto returnStmt = dynamic_cast<Chtholly::ReturnStatement*>(stmt);
        ASSERT_NE(returnStmt, nullptr);

        ASSERT_EQ(returnStmt->token.type, Chtholly::TokenType::Return);
    }
}
