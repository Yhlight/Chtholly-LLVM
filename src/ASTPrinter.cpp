#include "ASTPrinter.hpp"
#include <sstream>

std::string ASTPrinter::print(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::any ASTPrinter::visitAssignExpr(std::shared_ptr<AssignExpr> expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    return parenthesize("call " + std::any_cast<std::string>(expr->callee->accept(*this)), expr->arguments);
}

std::any ASTPrinter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(std::string)) {
            return "\"" + std::any_cast<std::string>(expr->value) + "\"";
        } else if (expr->value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr->value));
        } else if (expr->value.type() == typeid(bool)) {
            return std::any_cast<bool>(expr->value) ? "true" : "false";
        }
        return "nil";
    }
    return "nil";
}

std::any ASTPrinter::visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visitVariableExpr(std::shared_ptr<VariableExpr> expr) {
    return expr->name.lexeme;
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream builder;
    builder << "(" << name;
    for (const auto& expr : exprs) {
        builder << " ";
        builder << std::any_cast<std::string>(expr->accept(*this));
    }
    builder << ")";
    return builder.str();
}
