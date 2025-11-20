#include "Transpiler.hpp"
#include <sstream>

namespace chtholly {

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        if (stmt) { // Add null check
            ss << execute(stmt);
        }
    }
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<Binary> expr) {
    return evaluate(expr->left) + " " + expr->op.lexeme + " " + evaluate(expr->right);
}

std::any Transpiler::visit(std::shared_ptr<Grouping> expr) {
    return "(" + evaluate(expr->expression) + ")";
}

std::any Transpiler::visit(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(double)) {
            std::stringstream ss;
            ss << std::any_cast<double>(expr->value);
            return ss.str();
        }
        if (expr->value.type() == typeid(bool)) {
            return std::any_cast<bool>(expr->value) ? "true" : "false";
        }
        if (expr->value.type() == typeid(std::string)) {
            return "\"" + std::any_cast<std::string>(expr->value) + "\"";
        }
    }
    return "nullptr";
}

std::any Transpiler::visit(std::shared_ptr<Unary> expr) {
    return expr->op.lexeme + evaluate(expr->right);
}

std::any Transpiler::visit(std::shared_ptr<ExpressionStmt> stmt) {
    return evaluate(stmt->expression) + ";\n";
}

std::any Transpiler::visit(std::shared_ptr<VarStmt> stmt) {
    std::stringstream ss;
    ss << (stmt->is_mutable ? "" : "const ");
    // For now, we'll just use auto for type inference
    ss << "auto " << stmt->name.lexeme;
    if (stmt->initializer) {
        ss << " = " << evaluate(stmt->initializer);
    }
    ss << ";\n";
    return ss.str();
}

std::string Transpiler::evaluate(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::string Transpiler::execute(std::shared_ptr<Stmt> stmt) {
    return std::any_cast<std::string>(stmt->accept(*this));
}

} // namespace chtholly
