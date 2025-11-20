#include "ASTPrinter.hpp"
#include <sstream>
#include <string>

namespace chtholly {

std::string ASTPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::stringstream ss;
    for (const auto& stmt : statements) {
        if(stmt) {
             ss << std::any_cast<std::string>(stmt->accept(*this)) << std::endl;
        }
    }
    return ss.str();
}

std::string ASTPrinter::print(const std::shared_ptr<Expr>& expr) {
    if (!expr) return "";
    return std::any_cast<std::string>(expr->accept(*this));
}

std::string ASTPrinter::print(const std::shared_ptr<Stmt>& stmt) {
    if (!stmt) return "";
    return std::any_cast<std::string>(stmt->accept(*this));
}

// Expression visitors
std::any ASTPrinter::visit(const std::shared_ptr<Binary>& expr) {
    return parenthesize(expr->op.lexeme, expr->left, expr->right);
}

std::any ASTPrinter::visit(const std::shared_ptr<Grouping>& expr) {
    return parenthesize("group", expr->expression);
}

std::any ASTPrinter::visit(const std::shared_ptr<Literal>& expr) {
    if (!expr->value.has_value()) {
        return std::string("nil");
    }

    const auto& val = expr->value;
    if (val.type() == typeid(std::string)) {
        return std::any_cast<std::string>(val);
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

std::any ASTPrinter::visit(const std::shared_ptr<Unary>& expr) {
    return parenthesize(expr->op.lexeme, expr->right);
}

std::any ASTPrinter::visit(const std::shared_ptr<Variable>& expr) {
    return expr->name.lexeme;
}

std::any ASTPrinter::visit(const std::shared_ptr<Call>& expr) {
    return parenthesize("call", expr->callee);
}

std::any ASTPrinter::visit(const std::shared_ptr<Assign>& expr) {
    return parenthesize("= " + expr->name.lexeme, expr->value);
}


// Statement visitors
std::any ASTPrinter::visit(const std::shared_ptr<ExpressionStmt>& stmt) {
    return parenthesize("expr_stmt", stmt->expression);
}

std::any ASTPrinter::visit(const std::shared_ptr<VarStmt>& stmt) {
    if (stmt->initializer) {
        return parenthesize("var " + stmt->name.lexeme, stmt->initializer);
    }
    return parenthesize("var " + stmt->name.lexeme, nullptr);
}

std::any ASTPrinter::visit(const std::shared_ptr<BlockStmt>& stmt) {
    std::stringstream ss;
    ss << "(block";
    for (const auto& statement : stmt->statements) {
        ss << " " << print(statement);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<FunctionStmt>& stmt) {
    std::stringstream ss;
    ss << "(fun " << stmt->name.lexeme << " (";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        ss << stmt->params[i].lexeme;
        if (i < stmt->params.size() - 1) {
            ss << " ";
        }
    }
    ss << ") ";
    ss << print(stmt->body);
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<ReturnStmt>& stmt) {
    return parenthesize("return", stmt->value);
}

std::any ASTPrinter::visit(const std::shared_ptr<IfStmt>& stmt) {
    std::stringstream ss;
    ss << "(if " << print(stmt->condition) << " " << print(stmt->thenBranch);
    if (stmt->elseBranch) {
        ss << " " << print(stmt->elseBranch);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<WhileStmt>& stmt) {
    std::stringstream ss;
    ss << "(while " << print(stmt->condition) << " " << print(stmt->body) << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<ForStmt>& stmt) {
    std::stringstream ss;
    ss << "(for " << print(stmt->initializer) << " " << print(stmt->condition) << " " << print(stmt->increment) << " " << print(stmt->body) << ")";
    return ss.str();
}

// Helper methods for parenthesizing
std::string ASTPrinter::parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr) {
    std::stringstream ss;
    ss << "(" << name;
    if (expr) {
        ss << " " << print(expr);
    }
    ss << ")";
    return ss.str();
}

std::string ASTPrinter::parenthesize(const std::string& name, const std::shared_ptr<Expr>& expr1, const std::shared_ptr<Expr>& expr2) {
    std::stringstream ss;
    ss << "(" << name;
    if (expr1) {
        ss << " " << print(expr1);
    }
    if (expr2) {
        ss << " " << print(expr2);
    }
    ss << ")";
    return ss.str();
}
} // namespace chtholly
