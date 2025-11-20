#include "Transpiler.hpp"
#include <sstream>

namespace chtholly {

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream out;
    out << "#include <iostream>\n";
    out << "#include <string>\n";
    out << "#include <vector>\n\n";
    out << "int main() {\n";

    for (const auto& stmt : statements) {
        if(stmt != nullptr)
            out << "    " << std::any_cast<std::string>(stmt->accept(*this)) << "\n";
    }

    out << "    return 0;\n";
    out << "}\n";
    return out.str();
}

std::any Transpiler::visitAssignExpr(std::shared_ptr<Assign> expr) {
    return expr->name.lexeme + " = " + std::any_cast<std::string>(expr->value->accept(*this));
}

std::any Transpiler::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return "(" + std::any_cast<std::string>(expr->left->accept(*this)) + " " +
           expr->op.lexeme + " " +
           std::any_cast<std::string>(expr->right->accept(*this)) + ")";
}

std::any Transpiler::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return "(" + std::any_cast<std::string>(expr->expression->accept(*this)) + ")";
}

std::any Transpiler::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(expr->value));
        }
        if (expr->value.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr->value));
        }
        if (expr->value.type() == typeid(std::string)) {
            return "\\\"" + std::any_cast<std::string>(expr->value) + "\\\"";
        }
    }
    return "nullptr";
}

std::any Transpiler::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return expr->op.lexeme + std::any_cast<std::string>(expr->right->accept(*this));
}

std::any Transpiler::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any Transpiler::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) {
    return std::any_cast<std::string>(stmt->expression->accept(*this)) + ";";
}

std::any Transpiler::visitVarStmt(std::shared_ptr<VarStmt> stmt) {
    std::string type = (stmt->keyword.type == TokenType::LET) ? "auto const" : "auto";
    std::string initializer = "nullptr";
    if (stmt->initializer) {
        initializer = std::any_cast<std::string>(stmt->initializer->accept(*this));
    }
    return type + " " + stmt->name.lexeme + " = " + initializer + ";";
}

} // namespace chtholly
