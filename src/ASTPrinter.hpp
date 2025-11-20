#ifndef CHTHOLLY_AST_PRINTER_HPP
#define CHTHOLLY_AST_PRINTER_HPP

#include "AST.hpp"
#include <string>
#include <vector>

namespace chtholly {

class ASTPrinter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);
    std::string print(const std::shared_ptr<Expr>& expr);
    std::string print(const std::shared_ptr<Stmt>& stmt);

    // Expression visitors
    std::any visit(const std::shared_ptr<Binary>& expr) override;
    std::any visit(const std::shared_ptr<Grouping>& expr) override;
    std::any visit(const std::shared_ptr<Literal>& expr) override;
    std::any visit(const std::shared_ptr<Unary>& expr) override;
    std::any visit(const std::shared_ptr<Variable>& expr) override;
    std::any visit(const std::shared_ptr<Call>& expr) override;
    std::any visit(const std::shared_ptr<Assign>& expr) override;

    // Statement visitors
    std::any visit(const std::shared_ptr<ExpressionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<VarStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BlockStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FunctionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ReturnStmt>& stmt) override;
    std::any visit(const std::shared_ptr<IfStmt>& stmt) override;
    std::any visit(const std::shared_ptr<WhileStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ForStmt>& stmt) override;
    std::any visit(const std::shared_ptr<SwitchStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BreakStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FallthroughStmt>& stmt) override;


private:
    std::string parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr);
    std::string parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr1, const std::shared_ptr<Expr>& expr2);
};

} // namespace chtholly

#endif // CHTHOLLY_AST_PRINTER_HPP
