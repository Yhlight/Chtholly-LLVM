#include <gtest/gtest.h>
#include "../src/Parser.hpp"
#include "../src/Lexer.hpp"

TEST(ParserTest, StructInstantiation) {
    std::string source = "let p = Point{x: 0, y: 0};";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(statements[0]);
    ASSERT_NE(varStmt, nullptr);
    ASSERT_EQ(varStmt->name.lexeme, "p");

    auto structInitExpr = std::dynamic_pointer_cast<StructInitExpr>(varStmt->initializer);
    ASSERT_NE(structInitExpr, nullptr);
    ASSERT_EQ(structInitExpr->name.lexeme, "Point");
    ASSERT_EQ(structInitExpr->fields.size(), 2);

    ASSERT_EQ(structInitExpr->fields[0].first.lexeme, "x");
    auto xValue = std::dynamic_pointer_cast<Literal>(structInitExpr->fields[0].second);
    ASSERT_NE(xValue, nullptr);
    ASSERT_EQ(std::any_cast<int>(xValue->value), 0);

    ASSERT_EQ(structInitExpr->fields[1].first.lexeme, "y");
    auto yValue = std::dynamic_pointer_cast<Literal>(structInitExpr->fields[1].second);
    ASSERT_NE(yValue, nullptr);
    ASSERT_EQ(std::any_cast<int>(yValue->value), 0);
}

TEST(ParserTest, MemberAccess) {
    std::string source = "p.x;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(statements[0]);
    ASSERT_NE(exprStmt, nullptr);

    auto getExpr = std::dynamic_pointer_cast<GetExpr>(exprStmt->expression);
    ASSERT_NE(getExpr, nullptr);

    auto varExpr = std::dynamic_pointer_cast<Variable>(getExpr->object);
    ASSERT_NE(varExpr, nullptr);
    ASSERT_EQ(varExpr->name.lexeme, "p");
    ASSERT_EQ(getExpr->name.lexeme, "x");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
