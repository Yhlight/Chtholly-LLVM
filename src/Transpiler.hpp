#ifndef CHTHOLLY_TRANSPILER_HPP
#define CHTHOLLY_TRANSPILER_HPP

#include "AST.hpp"
#include "Stdlib.hpp"
#include <string>
#include <vector>
#include <set>

namespace chtholly {

class Transpiler : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
    Transpiler(std::string path);
    std::string transpile(const std::vector<std::shared_ptr<Stmt>>& statements);

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
    std::any visit(const std::shared_ptr<SwitchStmt>& stmt) override;
    std::any visit(const std::shared_ptr<BreakStmt>& stmt) override;
    std::any visit(const std::shared_ptr<FallthroughStmt>& stmt) override;
    std::any visit(const std::shared_ptr<EnumStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ClassStmt>& stmt) override;
    std::any visit(const std::shared_ptr<ImportStmt>& stmt) override;
    std::any visit(const std::shared_ptr<PackageStmt>& stmt) override;

private:
    std::string transpile(const std::vector<std::shared_ptr<Stmt>>& statements, bool is_main);
    std::string transpile(const std::shared_ptr<Stmt>& stmt);
    std::string transpile(const std::shared_ptr<Expr>& expr);
    std::string transpileType(const std::shared_ptr<Type>& type);
    std::string transpileParamType(const std::shared_ptr<Type>& type);

    std::set<std::string> included_files;
    std::string current_path;
    bool needs_functional = false;
    std::set<std::string> required_headers;
    Stdlib stdlib;
};

} // namespace chtholly

#endif // CHTHOLLY_TRANSPILER_HPP
