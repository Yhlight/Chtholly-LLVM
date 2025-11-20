#ifndef CHTHOLLY_AST_PRINTER_HPP
#define CHTHOLLY_AST_PRINTER_HPP

#include "AST.hpp"
#include <string>
#include <vector>
#include <memory>

namespace chtholly {

class ASTPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);
    std::string print(std::shared_ptr<Expr> expr);

    std::any visit(std::shared_ptr<Binary> expr) override;
    std::any visit(std::shared_ptr<Grouping> expr) override;
    std::any visit(std::shared_ptr<Literal> expr) override;
    std::any visit(std::shared_ptr<Unary> expr) override;
    std::any visit(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visit(std::shared_ptr<VarStmt> stmt) override;
    std::any visit(std::shared_ptr<BlockStmt> stmt) override;
    std::any visit(std::shared_ptr<IfStmt> stmt) override;
    std::any visit(std::shared_ptr<WhileStmt> stmt) override;
    std::any visit(std::shared_ptr<ForStmt> stmt) override;
    std::any visit(std::shared_ptr<Assign> expr) override;
    std::any visit(std::shared_ptr<Variable> expr) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
    std::string parenthesize_stmt(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts);
};

} // namespace chtholly

#endif //CHTHOLLY_AST_PRINTER_HPP
