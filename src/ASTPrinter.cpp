#include "ASTPrinter.hpp"
#include <sstream>

namespace chtholly {

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        if (stmt) {
            ss << std::any_cast<std::string>(stmt->accept(*this));
        }
    }
    return ss.str();
}

std::string ASTPrinter::print(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::any ASTPrinter::visit(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(double)) {
            std::stringstream ss;
            ss << std::any_cast<double>(expr->value);
            return ss.str();
        }
        if (expr->value.type() == typeid(bool)) {
            return std::string(std::any_cast<bool>(expr->value) ? "true" : "false");
        }
        if (expr->value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr->value);
        }
    }
    return std::string("nil");
}

std::any ASTPrinter::visit(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<ExpressionStmt> stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<VarStmt> stmt) {
    std::stringstream ss;
    ss << "(" << (stmt->is_mutable ? "mut" : "let") << " " << stmt->name.lexeme;
    if (stmt->type) {
        ss << ": " << stmt->type->lexeme;
    }
    if (stmt->initializer) {
        ss << " = " << print(stmt->initializer);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<BlockStmt> stmt) {
    return parenthesize_stmt("block", stmt->statements);
}

std::any ASTPrinter::visit(std::shared_ptr<IfStmt> stmt) {
    std::stringstream ss;
    ss << "(if " << print(stmt->condition);
    ss << " " << std::any_cast<std::string>(stmt->then_branch->accept(*this));
    if (stmt->else_branch) {
        ss << " else " << std::any_cast<std::string>(stmt->else_branch->accept(*this));
    }
    ss << ")";
    return ss.str();
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& expr : exprs) {
        ss << " ";
        ss << std::any_cast<std::string>(expr->accept(*this));
    }
    ss << ")";
    return ss.str();
}

std::string ASTPrinter::parenthesize_stmt(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& stmt : stmts) {
        ss << " ";
        ss << std::any_cast<std::string>(stmt->accept(*this));
    }
    ss << ")";
    return ss.str();
}

} // namespace chtholly
