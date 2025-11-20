#ifndef CHTHOLLY_TRANSPILER_HPP
#define CHTHOLLY_TRANSPILER_HPP

#include "AST.hpp"
#include <string>
#include <vector>
#include <memory>

class Transpiler : public ExprVisitor, public StmtVisitor {
public:
    std::string transpile(const std::vector<std::shared_ptr<Stmt>>& statements);

    std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    std::any visitCallExpr(std::shared_ptr<CallExpr> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
    std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) override;

    std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) override;
    std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) override;
    std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) override;

private:
    std::string evaluate(std::shared_ptr<Expr> expr);
    std::string execute(std::shared_ptr<Stmt> stmt);
};

#endif // CHTHOLLY_TRANSPILER_HPP
