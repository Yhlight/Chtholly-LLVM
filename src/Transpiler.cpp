#include "Transpiler.hpp"
#include <sstream>
#include <stdexcept>

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream builder;
    builder << "#include <iostream>\n";
    builder << "#include <any>\n";
    builder << "#include <vector>\n\n";
    builder << "int main() {\n";

    for (const auto& stmt : statements) {
        if (stmt) {
            builder << "    " << execute(stmt) << "\n";
        }
    }

    builder << "    return 0;\n";
    builder << "}\n";
    return builder.str();
}

std::string Transpiler::evaluate(std::shared_ptr<Expr> expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::string Transpiler::execute(std::shared_ptr<Stmt> stmt) {
    return std::any_cast<std::string>(stmt->accept(*this));
}

// Expression Visitors
std::any Transpiler::visitAssignExpr(std::shared_ptr<AssignExpr> expr) {
    return expr->name.lexeme + " = " + evaluate(expr->value);
}

std::any Transpiler::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) {
    return evaluate(expr->left) + " " + expr->op.lexeme + " " + evaluate(expr->right);
}

std::any Transpiler::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    // Placeholder implementation
    return std::string("/* call expression */");
}

std::any Transpiler::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) {
    return "(" + evaluate(expr->expression) + ")";
}

std::any Transpiler::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) {
    if (expr->value.type() == typeid(std::string)) {
        return "\"" + std::any_cast<std::string>(expr->value) + "\"";
    } else if (expr->value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(expr->value));
    } else if (expr->value.type() == typeid(bool)) {
        return std::any_cast<bool>(expr->value) ? "true" : "false";
    }
    return "nullptr";
}

std::any Transpiler::visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) {
    return evaluate(expr->left) + " " + expr->op.lexeme + " " + evaluate(expr->right);
}

std::any Transpiler::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) {
    return expr->op.lexeme + evaluate(expr->right);
}

std::any Transpiler::visitVariableExpr(std::shared_ptr<VariableExpr> expr) {
    return expr->name.lexeme;
}

// Statement Visitors
std::any Transpiler::visitBlockStmt(std::shared_ptr<BlockStmt> stmt) {
    std::stringstream builder;
    builder << "{\\n";
    for (const auto& statement : stmt->statements) {
        builder << "    " << execute(statement) << "\\n";
    }
    builder << "}";
    return builder.str();
}

std::any Transpiler::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) {
    return evaluate(stmt->expression) + ";";
}

std::any Transpiler::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) {
    // Placeholder implementation
    return std::string("/* function statement */");
}

std::any Transpiler::visitIfStmt(std::shared_ptr<IfStmt> stmt) {
    // Placeholder implementation
    return std::string("/* if statement */");
}

std::any Transpiler::visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) {
    // Placeholder implementation
    return std::string("/* return statement */");
}

std::any Transpiler::visitVarStmt(std::shared_ptr<VarStmt> stmt) {
    std::string type = stmt->isMutable ? "auto" : "const auto";
    std::string initializer = " = " + (stmt->initializer ? evaluate(stmt->initializer) : "nullptr");
    return type + " " + stmt->name.lexeme + initializer + ";";
}

std::any Transpiler::visitWhileStmt(std::shared_ptr<WhileStmt> stmt) {
    // Placeholder implementation
    return std::string("/* while statement */");
}
