#include "ASTPrinter.hpp"
#include <sstream>

namespace chtholly {

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream out;
    for (const auto& stmt : statements) {
        out << std::any_cast<std::string>(stmt->accept(*this));
    }
    return out.str();
}

std::any ASTPrinter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(expr->value));
        }
        if (expr->value.type() == typeid(double)) {
            std::string str = std::to_string(std::any_cast<double>(expr->value));
            return str.substr(0, str.find_last_not_of('0') + 1);
        }
        if (expr->value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr->value);
        }
    }
    return "nil";
}

std::any ASTPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visitVarStmt(std::shared_ptr<VarStmt> stmt) {
    if (stmt->initializer) {
        return parenthesize("var " + stmt->name.lexeme, {stmt->initializer});
    }
    return "(var " + stmt->name.lexeme + ")";
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream out;
    out << "(" << name;
    for (const auto& expr : exprs) {
        out << " " << std::any_cast<std::string>(expr->accept(*this));
    }
    out << ")";
    return out.str();
}

} // namespace chtholly
