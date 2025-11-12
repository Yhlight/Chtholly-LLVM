#ifndef CHTHOLLY_ASTPRINTER_H
#define CHTHOLLY_ASTPRINTER_H

#include "Expr.h"
#include "Stmt.h"
#include <string>
#include <vector>

namespace Chtholly {

class AstPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);

    std::any visitAssignExpr(Assign& expr) override;
    std::any visitLiteralExpr(Literal& expr) override;
    std::any visitVariableExpr(Variable& expr) override;

    std::any visitExpressionStmt(Expression& stmt) override;
    std::any visitVarStmt(Var& stmt) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
};

} // namespace Chtholly

#endif // CHTHOLLY_ASTPRINTER_H
