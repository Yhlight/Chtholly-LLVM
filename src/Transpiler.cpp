#include "Transpiler.hpp"
#include <sstream>

std::string Transpiler::transpile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    std::stringstream out;
    for (const auto& stmt : statements) {
        out << std::any_cast<std::string>(stmt->accept(*this));
    }
    return out.str();
}

std::any Transpiler::visit(const BinaryExpr& expr) {
    std::stringstream out;
    out << std::any_cast<std::string>(expr.left->accept(*this)) << " "
        << expr.op.lexeme << " "
        << std::any_cast<std::string>(expr.right->accept(*this));
    return out.str();
}

std::any Transpiler::visit(const GroupingExpr& expr) {
    std::stringstream out;
    out << "(" << std::any_cast<std::string>(expr.expression->accept(*this)) << ")";
    return out.str();
}

std::any Transpiler::visit(const LiteralExpr& expr) {
    if (!expr.value.has_value()) {
        return std::string("nullptr");
    }
    if (expr.value.type() == typeid(std::string)) {
        return "\"" + std::any_cast<std::string>(expr.value) + "\"";
    }
    if (expr.value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(expr.value));
    }
    if (expr.value.type() == typeid(bool)) {
        return std::any_cast<bool>(expr.value) ? "true" : "false";
    }
    return std::string("unhandled literal type");
}

std::any Transpiler::visit(const UnaryExpr& expr) {
    std::stringstream out;
    out << expr.op.lexeme << std::any_cast<std::string>(expr.right->accept(*this));
    return out.str();
}

std::any Transpiler::visit(const VarExpr& expr) {
    return expr.name.lexeme;
}

std::any Transpiler::visit(const AssignExpr& expr) {
    std::stringstream out;
    out << expr.name.lexeme << " = " << std::any_cast<std::string>(expr.value->accept(*this));
    return out.str();
}

std::any Transpiler::visit(const CallExpr& expr) {
    // Not implemented yet
    return std::string("");
}

std::any Transpiler::visit(const ExprStmt& stmt) {
    std::stringstream out;
    out << std::any_cast<std::string>(stmt.expression->accept(*this)) << ";\n";
    return out.str();
}

std::any Transpiler::visit(const VarStmt& stmt) {
    std::stringstream out;
    if (stmt.isMutable) {
        out << "auto " << stmt.name.lexeme;
    } else {
        out << "const auto " << stmt.name.lexeme;
    }

    if (stmt.initializer) {
        out << " = " << std::any_cast<std::string>(stmt.initializer->accept(*this));
    }
    out << ";\n";
    return out.str();
}

std::any Transpiler::visit(const BlockStmt& stmt) {
    std::stringstream out;
    out << "{\n";
    for (const auto& statement : stmt.statements) {
        out << std::any_cast<std::string>(statement->accept(*this));
    }
    out << "}\n";
    return out.str();
}

std::any Transpiler::visit(const IfStmt& stmt) {
    std::stringstream out;
    out << "if (" << std::any_cast<std::string>(stmt.condition->accept(*this)) << ") "
        << std::any_cast<std::string>(stmt.thenBranch->accept(*this));
    if (stmt.elseBranch) {
        out << "else " << std::any_cast<std::string>(stmt.elseBranch->accept(*this));
    }
    return out.str();
}

std::any Transpiler::visit(const WhileStmt& stmt) {
    std::stringstream out;
    out << "while (" << std::any_cast<std::string>(stmt.condition->accept(*this)) << ") "
        << std::any_cast<std::string>(stmt.body->accept(*this));
    return out.str();
}

std::any Transpiler::visit(const ReturnStmt& stmt) {
    std::stringstream out;
    out << "return";
    if (stmt.value) {
        out << " " << std::any_cast<std::string>(stmt.value->accept(*this));
    }
    out << ";\n";
    return out.str();
}
