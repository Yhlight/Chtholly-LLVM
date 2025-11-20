#ifndef CHTHOLLY_TRANSPILER_HPP
#define CHTHOLLY_TRANSPILER_HPP

#include "AST.hpp"
#include <string>
#include <vector>

namespace chtholly {

class Transpiler : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
    std::string transpile(const std::vector<std::shared_ptr<Stmt>>& statements);

    // Expression visitors
    std::any visit(const std::shared_ptr<Binary>& expr) override;
    std::any visit(const std::shared_ptr<Grouping>& expr) override;
    std::any visit(const std::shared_ptr<Literal>& expr) override;
    std::any visit(const std::shared_ptr<Unary>& expr) override;
    std::any visit(const std::shared_ptr<Variable>& expr) override;
    std::any visit(const std::shared_ptr<Call>& expr) override;

    // Statement visitors
    std::any visit(const std::shared_ptr<ExpressionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<VarStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BlockStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FunctionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ReturnStmt>& stmt) override;

private:
    std::string transpile(const std::shared_ptr<Stmt>& stmt);
    std::string transpile(const std::shared_ptr<Expr>& expr);
};

} // namespace chtholly

#endif // CHTHOLLY_TRANSPILER_HPP
