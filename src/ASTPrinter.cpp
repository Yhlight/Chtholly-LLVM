#include "ASTPrinter.hpp"
#include <sstream>

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        ss << std::any_cast<std::string>(stmt->accept(*this));
    }
    return ss.str();
}

std::any ASTPrinter::visitBinaryExpr(const std::shared_ptr<Binary>& expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visitUnaryExpr(const std::shared_ptr<Unary>& expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visitLiteralExpr(const std::shared_ptr<Literal>& expr) {
    if (expr->value.type() == typeid(nullptr)) return std::string("none");
    if (expr->value.type() == typeid(std::string)) return std::any_cast<std::string>(expr->value);
    if (expr->value.type() == typeid(int)) return std::to_string(std::any_cast<int>(expr->value));
    if (expr->value.type() == typeid(double)) return std::to_string(std::any_cast<double>(expr->value));
    if (expr->value.type() == typeid(bool)) return std::any_cast<bool>(expr->value) ? "true" : "false";
    return std::string("unknown literal");
}

std::any ASTPrinter::visitGroupingExpr(const std::shared_ptr<Grouping>& expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visitVariableExpr(const std::shared_ptr<Variable>& expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visitAssignExpr(const std::shared_ptr<Assign>& expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visitCallExpr(const std::shared_ptr<Call>& expr) {
    return parenthesize("call", {expr->callee});
}


std::any ASTPrinter::visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visitVarStmt(const std::shared_ptr<VarStmt>& stmt) {
    if (stmt->initializer != nullptr) {
        return parenthesize("var " + stmt->name.lexeme, {stmt->initializer});
    }
    return "(var " + stmt->name.lexeme + ")";
}

std::any ASTPrinter::visitIfStmt(const std::shared_ptr<IfStmt>& stmt) {
    if (stmt->elseBranch != nullptr) {
        return parenthesize("if-else", {stmt->condition});
    } else {
        return parenthesize("if", {stmt->condition});
    }
}

std::any ASTPrinter::visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt) {
    return parenthesize("while", {stmt->condition});
}

std::any ASTPrinter::visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) {
    return parenthesizeStmt("block", stmt->statements);
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

std::string ASTPrinter::parenthesizeStmt(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& stmt : stmts) {
        ss << " ";
        ss << std::any_cast<std::string>(stmt->accept(*this));
    }
    ss << ")";
    return ss.str();
}
