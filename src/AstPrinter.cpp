#include "AstPrinter.h"
#include <sstream>
#include <vector>

namespace Chtholly {

std::string AstPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        ss << std::any_cast<std::string>(stmt->accept(*this)) << std::endl;
    }
    return ss.str();
}

std::any AstPrinter::visitAssignExpr(Assign& expr) {
    return parenthesize("= " + expr.name.lexeme, {expr.value});
}

std::any AstPrinter::visitLiteralExpr(Literal& expr) {
    return std::string("LITERAL");
}

std::any AstPrinter::visitVariableExpr(Variable& expr) {
    return expr.name.lexeme;
}

std::any AstPrinter::visitExpressionStmt(Expression& stmt) {
    return std::any_cast<std::string>(stmt.expression->accept(*this)) + ";";
}

std::any AstPrinter::visitVarStmt(Var& stmt) {
    std::stringstream ss;
    ss << "var " << stmt.name.lexeme;
    if (stmt.initializer) {
        ss << " = " << std::any_cast<std::string>(stmt.initializer->accept(*this));
    }
    ss << ";";
    return ss.str();
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& expr : exprs) {
        ss << " " << std::any_cast<std::string>(expr->accept(*this));
    }
    ss << ")";
    return ss.str();
}

} // namespace Chtholly
