#include "Transpiler.hpp"
#include <sstream>

namespace chtholly {

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream out;
    out << "#include <iostream>\n";
    out << "#include <string>\n";
    out << "#include <vector>\n\n";

    for (const auto& stmt : statements) {
        if (auto func = std::dynamic_pointer_cast<FunctionStmt>(stmt)) {
            out << std::any_cast<std::string>(func->accept(*this)) << "\n";
        }
    }

    out << "int main() {\n";
    for (const auto& stmt : statements) {
        if (stmt != nullptr && !std::dynamic_pointer_cast<FunctionStmt>(stmt)) {
            out << "    " << std::any_cast<std::string>(stmt->accept(*this)) << "\n";
        }
    }
    out << "    return 0;\n";
    out << "}\n";
    return out.str();
}

std::any Transpiler::visitArrayLiteralExpr(std::shared_ptr<ArrayLiteralExpr> expr) {
    std::stringstream out;
    out << "std::vector<auto>({";
    for (size_t i = 0; i < expr->elements.size(); ++i) {
        out << std::any_cast<std::string>(expr->elements[i]->accept(*this));
        if (i < expr->elements.size() - 1) {
            out << ", ";
        }
    }
    out << "})";
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

std::any Transpiler::visitCallExpr(std::shared_ptr<CallExpr> expr) {
    std::stringstream out;
    out << std::any_cast<std::string>(expr->callee->accept(*this)) << "(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        out << std::any_cast<std::string>(expr->arguments[i]->accept(*this));
        if (i < expr->arguments.size() - 1) {
            out << ", ";
        }
    }
    out << ")";
    return out.str();
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

std::any Transpiler::visitSetExpr(std::shared_ptr<SetExpr> expr) {
    return std::any_cast<std::string>(expr->object->accept(*this)) + "[" +
           std::any_cast<std::string>(expr->index->accept(*this)) + "] = " +
           std::any_cast<std::string>(expr->value->accept(*this));
}

std::any Transpiler::visitSubscriptExpr(std::shared_ptr<SubscriptExpr> expr) {
    return std::any_cast<std::string>(expr->callee->accept(*this)) + "[" +
           std::any_cast<std::string>(expr->index->accept(*this)) + "]";
}

std::any Transpiler::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return expr->op.lexeme + std::any_cast<std::string>(expr->right->accept(*this));
}

std::any Transpiler::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any Transpiler::visitBlockStmt(std::shared_ptr<BlockStmt> stmt) {
    std::stringstream out;
    out << "{\n";
    for (const auto& statement : stmt->statements) {
        if(statement != nullptr)
            out << "    " << std::any_cast<std::string>(statement->accept(*this)) << "\n";
    }
    out << "}";
    return out.str();
}

std::any Transpiler::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) {
    return std::any_cast<std::string>(stmt->expression->accept(*this)) + ";";
}

std::any Transpiler::visitForStmt(std::shared_ptr<ForStmt> stmt) {
    std::stringstream out;
    out << "for (";
    if (stmt->initializer) {
        out << std::any_cast<std::string>(stmt->initializer->accept(*this));
    } else {
        out << ";";
    }
    if (stmt->condition) {
        out << " " << std::any_cast<std::string>(stmt->condition->accept(*this));
    }
    out << ";";
    if (stmt->increment) {
        out << " " << std::any_cast<std::string>(stmt->increment->accept(*this));
    }
    out << ") " << std::any_cast<std::string>(stmt->body->accept(*this));
    return out.str();
}

std::any Transpiler::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) {
    std::stringstream out;
    out << "auto " << stmt->name.lexeme << "(";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        out << "auto " << stmt->params[i].lexeme;
        if (i < stmt->params.size() - 1) {
            out << ", ";
        }
    }
    out << ") {\n";
    for (const auto& statement : stmt->body) {
         if(statement != nullptr)
            out << "    " << std::any_cast<std::string>(statement->accept(*this)) << "\n";
    }
    out << "}\n";
    return out.str();
}

std::any Transpiler::visitIfStmt(std::shared_ptr<IfStmt> stmt) {
    std::stringstream out;
    out << "if (" << std::any_cast<std::string>(stmt->condition->accept(*this)) << ") "
        << std::any_cast<std::string>(stmt->thenBranch->accept(*this));
    if (stmt->elseBranch) {
        out << " else " << std::any_cast<std::string>(stmt->elseBranch->accept(*this));
    }
    return out.str();
}

std::any Transpiler::visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) {
    if (stmt->value) {
        return "return " + std::any_cast<std::string>(stmt->value->accept(*this)) + ";";
    }
    return "return;";
}

std::any Transpiler::visitVarStmt(std::shared_ptr<VarStmt> stmt) {
    std::string type = (stmt->keyword.type == TokenType::LET) ? "auto const" : "auto";
    std::string initializer = "nullptr";
    if (stmt->initializer) {
        initializer = std::any_cast<std::string>(stmt->initializer->accept(*this));
    }
    return type + " " + stmt->name.lexeme + " = " + initializer + ";";
}

std::any Transpiler::visitWhileStmt(std::shared_ptr<WhileStmt> stmt) {
    return "while (" + std::any_cast<std::string>(stmt->condition->accept(*this)) + ") " +
           std::any_cast<std::string>(stmt->body->accept(*this));
}

} // namespace chtholly
