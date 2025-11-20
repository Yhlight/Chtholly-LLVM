#include "ASTPrinter.hpp"
#include <sstream>

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        ss << std::any_cast<std::string>(stmt->accept(*this)) << std::endl;
    }
    return ss.str();
}

std::string ASTPrinter::print(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::string ASTPrinter::print(std::shared_ptr<Stmt> stmt) {
    return std::any_cast<std::string>(stmt->accept(*this));
}

std::any ASTPrinter::visit(std::shared_ptr<Assign> expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visit(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(std::string)) {
            return "\"" + std::any_cast<std::string>(expr->value) + "\"";
        } else if (expr->value.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(expr->value));
        } else if (expr->value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr->value));
        }
    }
    return "nil";
}

std::any ASTPrinter::visit(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visit(std::shared_ptr<ExpressionStmt> stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<VarStmt> stmt) {
    if (stmt->initializer) {
        return parenthesize("var " + stmt->name.lexeme, {stmt->initializer});
    }
    return "(var " + stmt->name.lexeme + ")";
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& expr : exprs) {
        ss << " " << std::any_cast<std::string>(expr->accept(*this));
    }
    ss << ")";
    return ss.str();
}
