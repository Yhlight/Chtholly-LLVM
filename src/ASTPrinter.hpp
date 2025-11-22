#ifndef CHTHOLLY_AST_PRINTER_HPP
#define CHTHOLLY_AST_PRINTER_HPP

#include "AST.hpp"
#include <string>
#include <vector>
#include <any>

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
    std::any visit(const std::shared_ptr<ArrayLiteral>& expr) override;
    std::any visit(const std::shared_ptr<SubscriptExpr>& expr) override;
    std::any visit(const std::shared_ptr<ScopeExpr>& expr) override;
    std::any visit(const std::shared_ptr<LambdaExpr>& expr) override;
    std::any visit(const std::shared_ptr<GetExpr>& expr) override;
    std::any visit(const std::shared_ptr<SetExpr>& expr) override;
    std::any visit(const std::shared_ptr<ThisExpr>& expr) override;
    std::any visit(const std::shared_ptr<TypeCastExpr>& expr) override;

    // Statement visitors
    std::any visit(const std::shared_ptr<ExpressionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<VarStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BlockStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FunctionStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ReturnStmt>& stmt) override;
    std::any visit(const std::shared_ptr<IfStmt>& stmt) override;
    std::any visit(const std::shared_ptr<WhileStmt>& stmt) override;
    std::any visit(const std::shared_ptr<DoWhileStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ForStmt>& stmt) override;
    std::any visit(const std::shared_ptr<RangeForStmt>& stmt) override;
    std::any visit(const std::shared_ptr<SwitchStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BreakStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FallthroughStmt>& stmt) override;
    std::any visit(const std::shared_ptr<EnumStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ClassStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ImportStmt>& stmt) override;
    std::any visit(const std::shared_ptr<PackageStmt>& stmt) override;


private:
    std::string parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr);
    std::string parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr1, const std::shared_ptr<Expr>& expr2);
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts);
};

} // namespace chtholly

#endif // CHTHOLLY_AST_PRINTER_HPP
