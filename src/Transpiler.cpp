#include "Transpiler.hpp"
#include <sstream>
#include <stdexcept>

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    ss << "#include <iostream>\n";
    ss << "#include <string>\n\n";
    for (const auto& stmt : statements) {
        ss << std::any_cast<std::string>(stmt->accept(*this));
    }
    return ss.str();
}

// Expression Visitors
std::any Transpiler::visitBinaryExpr(const std::shared_ptr<Binary>& expr) {
    std::string left = std::any_cast<std::string>(expr->left->accept(*this));
    std::string right = std::any_cast<std::string>(expr->right->accept(*this));
    return "(" + left + " " + expr->op.lexeme + " " + right + ")";
}

std::any Transpiler::visitUnaryExpr(const std::shared_ptr<Unary>& expr) {
    std::string right = std::any_cast<std::string>(expr->right->accept(*this));
    return "(" + expr->op.lexeme + right + ")";
}

std::any Transpiler::visitLiteralExpr(const std::shared_ptr<Literal>& expr) {
    if (expr->value.type() == typeid(nullptr)) return std::string("nullptr");
    if (expr->value.type() == typeid(std::string)) return "\"" + std::any_cast<std::string>(expr->value) + "\"";
    if (expr->value.type() == typeid(int)) return std::to_string(std::any_cast<int>(expr->value));
    if (expr->value.type() == typeid(double)) return std::to_string(std::any_cast<double>(expr->value));
    if (expr->value.type() == typeid(bool)) return std::any_cast<bool>(expr->value) ? "true" : "false";
    throw std::runtime_error("Unknown literal type.");
}

std::any Transpiler::visitGroupingExpr(const std::shared_ptr<Grouping>& expr) {
    return "(" + std::any_cast<std::string>(expr->expression->accept(*this)) + ")";
}

std::any Transpiler::visitVariableExpr(const std::shared_ptr<Variable>& expr) {
    return expr->name.lexeme;
}

std::any Transpiler::visitAssignExpr(const std::shared_ptr<Assign>& expr) {
    std::string value = std::any_cast<std::string>(expr->value->accept(*this));
    return expr->name.lexeme + " = " + value;
}

std::any Transpiler::visitCallExpr(const std::shared_ptr<Call>& expr) {
    std::string callee = std::any_cast<std::string>(expr->callee->accept(*this));
    std::string args = "";
    for (const auto& arg : expr->arguments) {
        args += std::any_cast<std::string>(arg->accept(*this)) + ", ";
    }
    if (!args.empty()) {
        args.pop_back();
        args.pop_back();
    }
    // temporary hack for print
    if (callee == "print") {
        return "std::cout << " + args + " << std::endl";
    }

    return callee + "(" + args + ")";
}

// Statement Visitors
std::any Transpiler::visitExpressionStmt(const std::shared_ptr<ExpressionStmt>& stmt) {
    return "    " + std::any_cast<std::string>(stmt->expression->accept(*this)) + ";\n";
}

std::any Transpiler::visitVarStmt(const std::shared_ptr<VarStmt>& stmt) {
    std::string type = stmt->isMutable ? "auto" : "auto const";
    std::string initializer = "";
    if (stmt->initializer != nullptr) {
        initializer = " = " + std::any_cast<std::string>(stmt->initializer->accept(*this));
    }
    return "    " + type + " " + stmt->name.lexeme + initializer + ";\n";
}

std::any Transpiler::visitBlockStmt(const std::shared_ptr<BlockStmt>& stmt) {
    std::stringstream ss;
    ss << "{\n";
    for (const auto& statement : stmt->statements) {
        ss << std::any_cast<std::string>(statement->accept(*this));
    }
    ss << "}\n";
    return ss.str();
}

std::any Transpiler::visitIfStmt(const std::shared_ptr<IfStmt>& stmt) {
    std::stringstream ss;
    ss << "    if (" << std::any_cast<std::string>(stmt->condition->accept(*this)) << ") "
       << std::any_cast<std::string>(stmt->thenBranch->accept(*this));
    if (stmt->elseBranch != nullptr) {
        ss << "    else " << std::any_cast<std::string>(stmt->elseBranch->accept(*this));
    }
    return ss.str();
}

std::any Transpiler::visitWhileStmt(const std::shared_ptr<WhileStmt>& stmt) {
    std::stringstream ss;
    ss << "    while (" << std::any_cast<std::string>(stmt->condition->accept(*this)) << ") "
       << std::any_cast<std::string>(stmt->body->accept(*this));
    return ss.str();
}

std::any Transpiler::visitFunctionStmt(const std::shared_ptr<FunctionStmt>& stmt) {
    std::stringstream ss;
    ss << stmt->returnType.lexeme << " " << stmt->name.lexeme << "(";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        ss << stmt->params[i].type.lexeme << " " << stmt->params[i].name.lexeme;
        if (i < stmt->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ") ";
    ss << std::any_cast<std::string>(std::make_shared<BlockStmt>(stmt->body)->accept(*this));
    return ss.str();
}

std::any Transpiler::visitReturnStmt(const std::shared_ptr<ReturnStmt>& stmt) {
    if (stmt->value != nullptr) {
        return "    return " + std::any_cast<std::string>(stmt->value->accept(*this)) + ";\n";
    }
    return "    return;\n";
}

std::any Transpiler::visitForStmt(const std::shared_ptr<ForStmt>& stmt) {
    std::stringstream ss;
    ss << "    for (auto " << stmt->variable.lexeme << " : " << std::any_cast<std::string>(stmt->collection->accept(*this)) << ") "
       << std::any_cast<std::string>(stmt->body->accept(*this));
    return ss.str();
}

std::any Transpiler::visitStructStmt(const std::shared_ptr<StructStmt>& stmt) {
    std::stringstream ss;
    ss << "struct " << stmt->name.lexeme << " {\n";
    for (const auto& field : stmt->fields) {
        ss << "    " << field->type.lexeme << " " << field->name.lexeme << ";\n";
    }
    ss << "};\n";
    return ss.str();
}
