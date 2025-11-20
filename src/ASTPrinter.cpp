#include "ASTPrinter.hpp"
#include <sstream>

namespace chtholly {

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream out;
    for (const auto& stmt : statements) {
        if (stmt != nullptr)
            out << std::any_cast<std::string>(stmt->accept(*this));
    }
    return out.str();
}

std::any ASTPrinter::visitArrayLiteralExpr(std::shared_ptr<ArrayLiteralExpr> expr) {
    return parenthesize("array", expr->elements);
}

std::any ASTPrinter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    return parenthesize("call", {expr->callee});
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

std::any ASTPrinter::visitSetExpr(std::shared_ptr<SetExpr> expr) {
    return parenthesize("set", {expr->object, expr->index, expr->value});
}

std::any ASTPrinter::visitSubscriptExpr(std::shared_ptr<SubscriptExpr> expr) {
    return parenthesize("subscript", {expr->callee, expr->index});
}

std::any ASTPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visitBlockStmt(std::shared_ptr<BlockStmt> stmt) {
    std::stringstream out;
    out << "(block ";
    for (const auto& statement : stmt->statements) {
        if (statement != nullptr)
            out << std::any_cast<std::string>(statement->accept(*this));
    }
    out << ")";
    return out.str();
}

std::any ASTPrinter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visitForStmt(std::shared_ptr<ForStmt> stmt) {
    std::stringstream out;
    out << "(for " << std::any_cast<std::string>(stmt->initializer->accept(*this))
        << " " << std::any_cast<std::string>(stmt->condition->accept(*this))
        << " " << std::any_cast<std::string>(stmt->increment->accept(*this))
        << " " << std::any_cast<std::string>(stmt->body->accept(*this)) << ")";
    return out.str();
}

std::any ASTPrinter::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) {
    std::stringstream out;
    out << "(fn " << stmt->name.lexeme << " (";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        out << stmt->params[i].lexeme;
        if (i < stmt->params.size() - 1) out << " ";
    }
    out << ") ";
    for (const auto& statement : stmt->body) {
        if(statement != nullptr)
             out << std::any_cast<std::string>(statement->accept(*this));
    }
    out << ")";
    return out.str();
}

std::any ASTPrinter::visitIfStmt(std::shared_ptr<IfStmt> stmt) {
    std::stringstream out;
    out << "(if " << std::any_cast<std::string>(stmt->condition->accept(*this))
        << " " << std::any_cast<std::string>(stmt->thenBranch->accept(*this));
    if (stmt->elseBranch) {
        out << " " << std::any_cast<std::string>(stmt->elseBranch->accept(*this));
    }
    out << ")";
    return out.str();
}

std::any ASTPrinter::visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) {
    if (stmt->value) {
        return parenthesize("return", {stmt->value});
    }
    return "(return)";
}

std::any ASTPrinter::visitVarStmt(std::shared_ptr<VarStmt> stmt) {
    if (stmt->initializer) {
        return parenthesize("var " + stmt->name.lexeme, {stmt->initializer});
    }
    return "(var " + stmt->name.lexeme + ")";
}

std::any ASTPrinter::visitWhileStmt(std::shared_ptr<WhileStmt> stmt) {
    return parenthesize("while", {stmt->condition});
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
