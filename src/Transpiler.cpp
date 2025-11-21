#include "Transpiler.hpp"
#include <sstream>
#include <unordered_map>

namespace chtholly {

static std::string safe_any_to_string(const std::any& any) {
    try {
        return std::any_cast<std::string>(any);
    } catch (const std::bad_any_cast& e) {
        throw std::runtime_error("Transpiler: " + std::string(e.what()));
    }
}

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
    ss << "#include <iostream>\n";
    ss << "#include <string>\n";
    ss << "#include <vector>\n";
    ss << "#include <memory>\n\n";
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
    if (for_initializer) {
        return evaluate(stmt->expression);
    }
    return evaluate(stmt->expression) + ";\n";
}

std::any Transpiler::visit(std::shared_ptr<VarStmt> stmt) {
    std::stringstream ss;
    ss << (stmt->is_mutable ? "" : "const ");

    if (stmt->type) {
        ss << type_map.at(stmt->type->type) << " ";
    } else if (stmt->initializer && dynamic_cast<NewExpr*>(stmt->initializer.get())) {
        auto new_expr = dynamic_cast<NewExpr*>(stmt->initializer.get());
        ss << "std::unique_ptr<" << new_expr->name.lexeme << "> ";
    } else {
        ss << "auto ";
    }

    ss << stmt->name.lexeme;
    if (stmt->initializer) {
        ss << " = " << evaluate(stmt->initializer);
    }
    if (!for_initializer) {
        ss << ";\n";
    }
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
        ss << execute_for_initializer(stmt->initializer);
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

std::any Transpiler::visit(std::shared_ptr<ClassStmt> stmt) {
    std::stringstream ss;
    ss << "class " << stmt->name.lexeme << " {\n";
    ss << "public:\n";
    for (const auto& field : stmt->fields) {
        ss << execute(field);
    }
    for (const auto& method : stmt->methods) {
        ss << execute(method);
    }
    ss << "};\n";
    return ss.str();
}

std::any Transpiler::visit(std::shared_ptr<GetExpr> expr) {
    return evaluate(expr->object) + "." + expr->name.lexeme;
}

std::any Transpiler::visit(std::shared_ptr<SetExpr> expr) {
    return evaluate(expr->object) + "." + expr->name.lexeme + " = " + evaluate(expr->value);
}

std::any Transpiler::visit(std::shared_ptr<ThisExpr> expr) {
    return "this";
}

std::any Transpiler::visit(std::shared_ptr<NewExpr> expr) {
    std::stringstream ss;
    ss << "std::make_unique<" << expr->name.lexeme << ">(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        ss << evaluate(expr->arguments[i]);
        if (i < expr->arguments.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}

std::string Transpiler::evaluate(std::shared_ptr<Expr> expr) {
    return safe_any_to_string(expr->accept(*this));
}

std::string Transpiler::execute(std::shared_ptr<Stmt> stmt) {
    return safe_any_to_string(stmt->accept(*this));
}

std::string Transpiler::execute_for_initializer(std::shared_ptr<Stmt> stmt) {
    for_initializer = true;
    std::string result = safe_any_to_string(stmt->accept(*this));
    for_initializer = false;
    return result;
}

} // namespace chtholly
