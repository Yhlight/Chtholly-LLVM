#include "Transpiler.hpp"
#include <sstream>
#include <unordered_map>

namespace chtholly {

static const std::unordered_map<TokenType, std::string> type_map = {
    {TokenType::INT, "int"},
    {TokenType::I8, "int8_t"},
    {TokenType::I16, "int16_t"},
    {TokenType::I32, "int32_t"},
    {TokenType::I64, "int64_t"},
    {TokenType::U8, "uint8_t"},
    {TokenType::U16, "uint16_t"},
    {TokenType::U32, "uint32_t"},
    {TokenType::U64, "uint64_t"},
    {TokenType::CHAR, "char"},
    {TokenType::DOUBLE, "double"},
    {TokenType::FLOAT, "float"},
    {TokenType::LONG_DOUBLE, "long double"},
    {TokenType::VOID, "void"},
    {TokenType::BOOL, "bool"},
    {TokenType::STRING, "std::string"}
};

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        if (stmt) {
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
            return std::string(std::any_cast<bool>(expr->value) ? "true" : "false");
        }
        if (expr->value.type() == typeid(std::string)) {
            return "\"" + std::any_cast<std::string>(expr->value) + "\"";
        }
    }
    return std::string("nullptr");
}

std::any Transpiler::visit(std::shared_ptr<Unary> expr) {
    return expr->op.lexeme + evaluate(expr->right);
}

std::any Transpiler::visit(std::shared_ptr<Assign> expr) {
    return expr->name.lexeme + " = " + evaluate(expr->value);
}

std::any Transpiler::visit(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any Transpiler::visit(std::shared_ptr<CallExpr> expr) {
    std::stringstream ss;
    ss << evaluate(expr->callee) << "(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        ss << evaluate(expr->arguments[i]);
        if (i < expr->arguments.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<ExpressionStmt> stmt) {
    return evaluate(stmt->expression) + ";\n";
}

std::any Transpiler::visit(std::shared_ptr<VarStmt> stmt) {
    std::stringstream ss;
    ss << (stmt->is_mutable ? "" : "const ");

    if (stmt->type) {
        ss << type_map.at(stmt->type->type) << " ";
    } else {
        ss << "auto ";
    }

    ss << stmt->name.lexeme;
    if (stmt->initializer) {
        ss << " = " << evaluate(stmt->initializer);
    }
    ss << ";\n";
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<BlockStmt> stmt) {
    std::stringstream ss;
    ss << "{\n";
    for (const auto& statement : stmt->statements) {
        ss << execute(statement);
    }
    ss << "}\n";
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<IfStmt> stmt) {
    std::stringstream ss;
    ss << "if (" << evaluate(stmt->condition) << ") " << execute(stmt->then_branch);
    if (stmt->else_branch) {
        ss << "else " << execute(stmt->else_branch);
    }
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<WhileStmt> stmt) {
    std::stringstream ss;
    ss << "while (" << evaluate(stmt->condition) << ") " << execute(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<ForStmt> stmt) {
    std::stringstream ss;
    ss << "for (";
    if (stmt->initializer) {
        std::string init = execute(stmt->initializer);
        init.pop_back(); // remove trailing newline
        init.pop_back(); // remove trailing semicolon
        ss << init;
    }
    ss << ";";

    if (stmt->condition) {
        ss << " " << evaluate(stmt->condition);
    }
    ss << ";";

    if (stmt->increment) {
        ss << " " << evaluate(stmt->increment);
    }
    ss << ") " << execute(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<FunctionStmt> stmt) {
    std::stringstream ss;
    if (stmt->return_type) {
        ss << type_map.at(stmt->return_type->type) << " ";
    } else {
        ss << "void ";
    }
    ss << stmt->name.lexeme << "(";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        ss << type_map.at(stmt->param_types[i].type) << " " << stmt->params[i].lexeme;
        if (i < stmt->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ") " << execute(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<ReturnStmt> stmt) {
    std::stringstream ss;
    ss << "return";
    if (stmt->value) {
        ss << " " << evaluate(stmt->value);
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

std::string Transpiler::execute_for_initializer(std::shared_ptr<Stmt> stmt) {
    return std::any_cast<std::string>(stmt->accept(*this));
}

} // namespace chtholly
