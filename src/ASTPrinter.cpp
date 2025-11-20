#include "ASTPrinter.hpp"
#include <sstream>

std::string ASTPrinter::print(const std::vector<std::unique_ptr<Stmt>>& statements) {
    std::stringstream out;
    for (const auto& stmt : statements) {
        out << std::any_cast<std::string>(stmt->accept(*this));
    }
    return out.str();
}

std::any ASTPrinter::visit(const BinaryExpr& expr) {
    return parenthesize(expr.op.lexeme, {expr.left.get(), expr.right.get()});
}

std::any ASTPrinter::visit(const GroupingExpr& expr) {
    return parenthesize("group", {expr.expression.get()});
}

std::any ASTPrinter::visit(const LiteralExpr& expr) {
    if (!expr.value.has_value()) {
        return std::string("nil");
    }
    if (expr.value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(expr.value);
    }
    if (expr.value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(expr.value));
    }
    if (expr.value.type() == typeid(bool)) {
        return std::any_cast<bool>(expr.value) ? std::string("true") : std::string("false");
    }
    return std::string("unhandled literal type");
}

std::any ASTPrinter::visit(const UnaryExpr& expr) {
    return parenthesize(expr.op.lexeme, {expr.right.get()});
}

std::any ASTPrinter::visit(const VarExpr& expr) {
    return expr.name.lexeme;
}

std::any ASTPrinter::visit(const AssignExpr& expr) {
    // This creates a temporary VarExpr just for printing.
    VarExpr tempVar(expr.name);
    return parenthesize("=", {&tempVar, expr.value.get()});
}

std::any ASTPrinter::visit(const CallExpr& expr) {
    std::vector<const Expr*> exprs;
    exprs.push_back(expr.callee.get());
    for (const auto& arg : expr.arguments) {
        exprs.push_back(arg.get());
    }
    return parenthesize("call", exprs);
}

std::any ASTPrinter::visit(const ExprStmt& stmt) {
    return parenthesize(";", {stmt.expression.get()});
}

std::any ASTPrinter::visit(const VarStmt& stmt) {
    if (stmt.initializer) {
        return parenthesize("var " + stmt.name.lexeme, {stmt.initializer.get()});
    }
    return std::string("(var " + stmt.name.lexeme + ")");
}

std::any ASTPrinter::visit(const BlockStmt& stmt) {
    std::stringstream out;
    out << "(block ";
    for (const auto& statement : stmt.statements) {
        out << std::any_cast<std::string>(statement->accept(*this));
    }
    out << ")";
    return out.str();
}

std::any ASTPrinter::visit(const IfStmt& stmt) {
    std::stringstream out;
    out << "(if " << std::any_cast<std::string>(stmt.condition->accept(*this)) << " "
        << std::any_cast<std::string>(stmt.thenBranch->accept(*this));
    if (stmt.elseBranch) {
        out << " " << std::any_cast<std::string>(stmt.elseBranch->accept(*this));
    }
    out << ")";
    return out.str();
}

std::any ASTPrinter::visit(const WhileStmt& stmt) {
    std::stringstream out;
    out << "(while " << std::any_cast<std::string>(stmt.condition->accept(*this)) << " "
        << std::any_cast<std::string>(stmt.body->accept(*this)) << ")";
    return out.str();
}

std::any ASTPrinter::visit(const ReturnStmt& stmt) {
    if (stmt.value) {
        return parenthesize("return", {stmt.value.get()});
    }
    return std::string("(return)");
}

std::any ASTPrinter::parenthesize(const std::string& name, const std::vector<const Expr*>& exprs) {
    std::stringstream out;
    out << "(" << name;
    for (const auto& expr : exprs) {
        out << " " << std::any_cast<std::string>(expr->accept(*this));
    }
    out << ")";
    return out.str();
}
