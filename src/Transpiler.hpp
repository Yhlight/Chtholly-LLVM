#ifndef CHTHOLLY_TRANSPILER_HPP
#define CHTHOLLY_TRANSPILER_HPP

#include "AST.hpp"
#include <string>
#include <vector>
#include <memory>

namespace chtholly {

class Transpiler : public ExprVisitor, public StmtVisitor {
public:
    std::string transpile(const std::vector<std::shared_ptr<Stmt>>& statements);

private:
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
    std::any visit(std::shared_ptr<CallExpr> expr) override;
    std::any visit(std::shared_ptr<FunctionStmt> stmt) override;

    std::string evaluate(std::shared_ptr<Expr> expr);
    std::string execute(std::shared_ptr<Stmt> stmt);
    std::string execute_for_initializer(std::shared_ptr<Stmt> stmt);
};

} // namespace chtholly

#endif //CHTHOLLY_TRANSPILER_HPP
