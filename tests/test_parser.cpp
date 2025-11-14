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

TEST(ParserTest, StructWithMethod) {
    std::string source =
        "struct Point {                  \n"
        "    let x: int;                  \n"
        "    let y: int;                  \n"
        "                                 \n"
        "    add(&self, other: Point) -> Point { \n"
        "        return Point{x: self.x + other.x, y: self.y + other.y}; \n"
        "    }                            \n"
        "}                                \n";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto structStmt = std::dynamic_pointer_cast<StructStmt>(statements[0]);
    ASSERT_NE(structStmt, nullptr);
    ASSERT_EQ(structStmt->name.lexeme, "Point");
    ASSERT_EQ(structStmt->fields.size(), 2);
    ASSERT_EQ(structStmt->methods.size(), 1);

    auto method = structStmt->methods[0];
    ASSERT_EQ(method->name.lexeme, "add");
    ASSERT_EQ(method->params.size(), 2);
    ASSERT_EQ(method->params[0].name.lexeme, "self");
    ASSERT_EQ(method->params[1].name.lexeme, "other");
    ASSERT_EQ(method->returnType.lexeme, "Point");
}

TEST(ParserTest, MemberMethodCall) {
    std::string source = "p.add(other);";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(statements[0]);
    ASSERT_NE(exprStmt, nullptr);

    auto callExpr = std::dynamic_pointer_cast<Call>(exprStmt->expression);
    ASSERT_NE(callExpr, nullptr);
    ASSERT_EQ(callExpr->arguments.size(), 1);

    auto getExpr = std::dynamic_pointer_cast<GetExpr>(callExpr->callee);
    ASSERT_NE(getExpr, nullptr);

    auto varExpr = std::dynamic_pointer_cast<Variable>(getExpr->object);
    ASSERT_NE(varExpr, nullptr);
    ASSERT_EQ(varExpr->name.lexeme, "p");

    ASSERT_EQ(getExpr->name.lexeme, "add");
}

TEST(ParserTest, StaticMethodCall) {
    std::string source = "Point::new();";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    ASSERT_EQ(statements.size(), 1);
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(statements[0]);
    ASSERT_NE(exprStmt, nullptr);

    auto callExpr = std::dynamic_pointer_cast<Call>(exprStmt->expression);
    ASSERT_NE(callExpr, nullptr);
    ASSERT_EQ(callExpr->arguments.size(), 0);

    auto staticGetExpr = std::dynamic_pointer_cast<StaticGetExpr>(callExpr->callee);
    ASSERT_NE(staticGetExpr, nullptr);

    ASSERT_EQ(staticGetExpr->klass.lexeme, "Point");
    ASSERT_EQ(staticGetExpr->name.lexeme, "new");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
