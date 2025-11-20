#include "Transpiler.hpp"
#include <sstream>

namespace chtholly {

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    ss << "#include <iostream>" << std::endl;
    ss << "#include <string>" << std::endl;
    ss << "#include <vector>" << std::endl;
    ss << std::endl;

    for (const auto& stmt : statements) {
        if (stmt) {
            ss << transpile(stmt);
        }
    }
    return ss.str();
}

std::string Transpiler::transpile(const std::shared_ptr<Stmt>& stmt) {
    return std::any_cast<std::string>(stmt->accept(*this));
}

std::string Transpiler::transpile(const std::shared_ptr<Expr>& expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

// Expression visitors
std::any Transpiler::visit(const std::shared_ptr<Binary>& expr) {
    return transpile(expr->left) + " " + expr->op.lexeme + " " + transpile(expr->right);
}

std::any Transpiler::visit(const std::shared_ptr<Grouping>& expr) {
    return "(" + transpile(expr->expression) + ")";
}

std::any Transpiler::visit(const std::shared_ptr<Literal>& expr) {
    if (!expr->value.has_value()) {
        return std::string("nullptr");
    }

    const auto& val = expr->value;
    if (val.type() == typeid(std::string)) {
        return "\"" + std::any_cast<std::string>(val) + "\"";
    }
    if (val.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(val));
    }
    if (val.type() == typeid(double)) {
        std::stringstream ss;
        ss << std::any_cast<double>(val);
        return ss.str();
    }
    if (val.type() == typeid(bool)) {
        return std::string(std::any_cast<bool>(val) ? "true" : "false");
    }

    return std::string("unknown literal");
}

std::any Transpiler::visit(const std::shared_ptr<Unary>& expr) {
    return expr->op.lexeme + transpile(expr->right);
}

std::any Transpiler::visit(const std::shared_ptr<Variable>& expr) {
    return expr->name.lexeme;
}

std::any Transpiler::visit(const std::shared_ptr<Call>& expr) {
    std::stringstream ss;
    ss << transpile(expr->callee) << "(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        ss << transpile(expr->arguments[i]);
        if (i < expr->arguments.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<Assign>& expr) {
    return transpile(expr->target) + " = " + transpile(expr->value);
}

std::any Transpiler::visit(const std::shared_ptr<ArrayLiteral>& expr) {
    std::stringstream ss;
    ss << "{";
    for (size_t i = 0; i < expr->elements.size(); ++i) {
        ss << transpile(expr->elements[i]);
        if (i < expr->elements.size() - 1) {
            ss << ", ";
        }
    }
    ss << "}";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<SubscriptExpr>& expr) {
    return transpile(expr->name) + "[" + transpile(expr->index) + "]";
}

// Statement visitors
std::any Transpiler::visit(const std::shared_ptr<ExpressionStmt>& stmt) {
    return transpile(stmt->expression) + ";\n";
}

std::string transpileType(const std::shared_ptr<Type>& type) {
    if (type->getKind() == TypeKind::PRIMITIVE) {
        auto primitive = std::dynamic_pointer_cast<PrimitiveType>(type);
        return primitive->name;
    }
    if (type->getKind() == TypeKind::ARRAY) {
        auto array = std::dynamic_pointer_cast<ArrayType>(type);
        return "std::vector<" + transpileType(array->element_type) + ">";
    }
    return "auto";
}

std::any Transpiler::visit(const std::shared_ptr<VarStmt>& stmt) {
    std::stringstream ss;
    if (stmt->type) {
        ss << transpileType(stmt->type) << " " << stmt->name.lexeme;
    } else {
        ss << "auto " << stmt->name.lexeme;
    }

    if (stmt->initializer) {
        ss << " = " << transpile(stmt->initializer);
    }
    ss << ";\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<BlockStmt>& stmt) {
    std::stringstream ss;
    ss << "{\n";
    for (const auto& statement : stmt->statements) {
        ss << transpile(statement);
    }
    ss << "}\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<FunctionStmt>& stmt) {
    std::stringstream ss;
    if (stmt->name.lexeme == "main") {
        ss << "int main(int argc, char* argv[]) ";
    } else {
        ss << "auto " << stmt->name.lexeme << "(";
        for (size_t i = 0; i < stmt->params.size(); ++i) {
            ss << "auto " << stmt->params[i].lexeme;
            if (i < stmt->params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ") ";
    }
    ss << transpile(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<ReturnStmt>& stmt) {
    std::stringstream ss;
    ss << "return";
    if (stmt->value) {
        ss << " " << transpile(stmt->value);
    }
    ss << ";\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<IfStmt>& stmt) {
    std::stringstream ss;
    ss << "if (" << transpile(stmt->condition) << ") " << transpile(stmt->thenBranch);
    if (stmt->elseBranch) {
        ss << "else " << transpile(stmt->elseBranch);
    }
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<WhileStmt>& stmt) {
    std::stringstream ss;
    ss << "while (" << transpile(stmt->condition) << ") " << transpile(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<ForStmt>& stmt) {
    std::stringstream ss;
    ss << "for (";
    if (stmt->initializer) {
        if (auto var_stmt = std::dynamic_pointer_cast<VarStmt>(stmt->initializer)) {
            ss << "auto " << var_stmt->name.lexeme;
            if (var_stmt->initializer) {
                ss << " = " << transpile(var_stmt->initializer);
            }
        } else if (auto expr_stmt = std::dynamic_pointer_cast<ExpressionStmt>(stmt->initializer)) {
            ss << transpile(expr_stmt->expression);
        }
    }
    ss << ";";

    if (stmt->condition) {
        ss << " " << transpile(stmt->condition);
    }
    ss << ";";

    if (stmt->increment) {
        ss << " " << transpile(stmt->increment);
    }
    ss << ") " << transpile(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<SwitchStmt>& stmt) {
    std::stringstream ss;
    ss << "switch (" << transpile(stmt->expression) << ") {\n";
    for (const auto& case_ : stmt->cases) {
        ss << "case " << transpile(case_.condition) << ":\n";
        ss << transpile(case_.body);
    }
    ss << "}\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<BreakStmt>& stmt) {
    return std::string("break;\n");
}

std::any Transpiler::visit(const std::shared_ptr<FallthroughStmt>& stmt) {
    return std::string("[[fallthrough]];\n");
}

} // namespace chtholly
