#include "ASTPrinter.hpp"
#include <sstream>

namespace chtholly {

static std::string safe_any_to_string(const std::any& any) {
    try {
        return std::any_cast<std::string>(any);
    } catch (const std::bad_any_cast& e) {
        throw std::runtime_error("ASTPrinter: " + std::string(e.what()));
    }
}

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        if (stmt) {
            ss << safe_any_to_string(stmt->accept(*this));
        }
    }
    return ss.str();
}

std::string ASTPrinter::print(std::shared_ptr<Expr> expr) {
    return safe_any_to_string(expr->accept(*this));
}

std::any ASTPrinter::visit(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", {expr->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<Literal> expr) {
    if (expr->value.has_value()) {
        if (expr->value.type() == typeid(double)) {
            std::stringstream ss;
            ss << std::any_cast<double>(expr->value);
            return ss.str();
        }
        if (expr->value.type() == typeid(int)) {
            std::stringstream ss;
            ss << std::any_cast<int>(expr->value);
            return ss.str();
        }
        if (expr->value.type() == typeid(bool)) {
            return std::string(std::any_cast<bool>(expr->value) ? "true" : "false");
        }
        if (expr->value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(expr->value);
        }
        if (expr->value.type() == typeid(char)) {
            std::stringstream ss;
            ss << "'" << std::any_cast<char>(expr->value) << "'";
            return ss.str();
        }
    }
    return std::string("nil");
}

std::any ASTPrinter::visit(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::any ASTPrinter::visit(std::shared_ptr<Assign> expr) {
    return parenthesize("= " + expr->name.lexeme, {expr->value});
}

std::any ASTPrinter::visit(std::shared_ptr<Variable> expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visit(std::shared_ptr<CallExpr> expr) {
    std::stringstream ss;
    ss << "(call " << print(expr->callee) << "(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        ss << print(expr->arguments[i]);
        if (i < expr->arguments.size() - 1) {
            ss << ", ";
        }
    }
    ss << "))";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<ExpressionStmt> stmt) {
    return parenthesize(";", {stmt->expression});
}

std::any ASTPrinter::visit(std::shared_ptr<VarStmt> stmt) {
    std::stringstream ss;
    ss << "(" << (stmt->is_mutable ? "mut" : "let") << " " << stmt->name.lexeme;
    if (stmt->type) {
        ss << ": " << stmt->type->lexeme;
    }
    if (stmt->initializer) {
        ss << " = " << print(stmt->initializer);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<BlockStmt> stmt) {
    return parenthesize_stmt("block", stmt->statements);
}

std::any ASTPrinter::visit(std::shared_ptr<IfStmt> stmt) {
    std::stringstream ss;
    ss << "(if " << print(stmt->condition);
    ss << " " << safe_any_to_string(stmt->then_branch->accept(*this));
    if (stmt->else_branch) {
        ss << " else " << safe_any_to_string(stmt->else_branch->accept(*this));
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<WhileStmt> stmt) {
    std::stringstream ss;
    ss << "(while " << print(stmt->condition);
    ss << " " << safe_any_to_string(stmt->body->accept(*this));
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<ForStmt> stmt) {
    std::stringstream ss;
    ss << "(for ";
    if (stmt->initializer) {
        ss << safe_any_to_string(stmt->initializer->accept(*this));
    }
    ss << " " << print(stmt->condition);
    ss << " " << print(stmt->increment);
    ss << " " << safe_any_to_string(stmt->body->accept(*this));
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<FunctionStmt> stmt) {
    std::stringstream ss;
    ss << "(fn " << stmt->name.lexeme << "(";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        ss << stmt->params[i].lexeme << ": " << stmt->param_types[i].lexeme;
        if (i < stmt->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    if (stmt->return_type) {
        ss << ": " << stmt->return_type->lexeme;
    }
    ss << " " << safe_any_to_string(stmt->body->accept(*this));
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<ReturnStmt> stmt) {
    if (stmt->value) {
        return parenthesize("return", {stmt->value});
    }
    return "(return)";
}

std::any ASTPrinter::visit(std::shared_ptr<ClassStmt> stmt) {
    std::stringstream ss;
    ss << "(class " << stmt->name.lexeme;
    for (const auto& field : stmt->fields) {
        ss << " " << safe_any_to_string(field->accept(*this));
    }
    for (const auto& method : stmt->methods) {
        ss << " " << safe_any_to_string(method->accept(*this));
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(std::shared_ptr<GetExpr> expr) {
    return parenthesize("." + expr->name.lexeme, {expr->object});
}

std::any ASTPrinter::visit(std::shared_ptr<SetExpr> expr) {
    return parenthesize("= ." + expr->name.lexeme, {expr->object, expr->value});
}

std::any ASTPrinter::visit(std::shared_ptr<ThisExpr> expr) {
    return "this";
}

std::any ASTPrinter::visit(std::shared_ptr<NewExpr> expr) {
    std::stringstream ss;
    ss << "(new " << expr->name.lexeme << "(";
    for (size_t i = 0; i < expr->arguments.size(); ++i) {
        ss << print(expr->arguments[i]);
        if (i < expr->arguments.size() - 1) {
            ss << ", ";
        }
    }
    ss << "))";
    return ss.str();
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& expr : exprs) {
        ss << " ";
        ss << safe_any_to_string(expr->accept(*this));
    }
    ss << ")";
    return ss.str();
}

std::string ASTPrinter::parenthesize_stmt(const std::string& name, const std::vector<std::shared_ptr<Stmt>>& stmts) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& stmt : stmts) {
        ss << " ";
        ss << safe_any_to_string(stmt->accept(*this));
    }
    ss << ")";
    return ss.str();
}

} // namespace chtholly
