#include "ASTPrinter.hpp"
#include <sstream>
#include <string>

namespace chtholly {

std::string printType(const std::shared_ptr<Type>& type) {
    if (!type) return "";
    if (type->getKind() == TypeKind::PRIMITIVE) {
        auto primitive = std::dynamic_pointer_cast<PrimitiveType>(type);
        return primitive->name;
    }
    if (type->getKind() == TypeKind::ARRAY) {
        auto array = std::dynamic_pointer_cast<ArrayType>(type);
        return printType(array->element_type) + "[]";
    }
    if (type->getKind() == TypeKind::FUNCTION) {
        auto func = std::dynamic_pointer_cast<FunctionType>(type);
        std::stringstream ss;
        ss << "(";
        for (size_t i = 0; i < func->param_types.size(); ++i) {
            ss << printType(func->param_types[i]);
            if (i < func->param_types.size() - 1) {
                ss << ", ";
            }
        }
        ss << "): " << printType(func->return_type);
        return ss.str();
    }
    return "unknown_type";
}

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
    std::stringstream ss;
    ss << "(call " << print(expr->callee);
    if (!expr->type_arguments.empty()) {
        ss << "<";
        for (size_t i = 0; i < expr->type_arguments.size(); ++i) {
            ss << printType(expr->type_arguments[i]);
            if (i < expr->type_arguments.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">";
    }
    for (const auto& arg : expr->arguments) {
        ss << " " << print(arg);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<Assign>& expr) {
    return parenthesize("= " + print(expr->target), expr->value);
}

std::any ASTPrinter::visit(const std::shared_ptr<ArrayLiteral>& expr) {
    std::stringstream ss;
    ss << "(array";
    for (const auto& element : expr->elements) {
        ss << " " << print(element);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<SubscriptExpr>& expr) {
    return parenthesize("subscript " + print(expr->name), expr->index);
}

std::any ASTPrinter::visit(const std::shared_ptr<ScopeExpr>& expr) {
    return parenthesize(":: " + print(expr->left), std::make_shared<Variable>(expr->name));
}

std::any ASTPrinter::visit(const std::shared_ptr<LambdaExpr>& expr) {
    std::stringstream ss;
    ss << "(lambda (";
    for (size_t i = 0; i < expr->params.size(); ++i) {
        ss << expr->params[i].name.lexeme << ": " << printType(expr->params[i].type);
        if (i < expr->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    if (expr->return_type) {
        ss << ": " << printType(expr->return_type);
    }
    ss << " " << print(expr->body);
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<GetExpr>& expr) {
    return parenthesize("get " + expr->name.lexeme, expr->object);
}

std::any ASTPrinter::visit(const std::shared_ptr<SetExpr>& expr) {
    return parenthesize("set " + expr->name.lexeme, expr->object, expr->value);
}

std::any ASTPrinter::visit(const std::shared_ptr<ThisExpr>& expr) {
    return std::string("this");
}


// Statement visitors
std::any ASTPrinter::visit(const std::shared_ptr<ExpressionStmt>& stmt) {
    return parenthesize("expr_stmt", stmt->expression);
}

std::any ASTPrinter::visit(const std::shared_ptr<VarStmt>& stmt) {
    std::stringstream ss;
    ss << "(var " << stmt->name.lexeme;
    if (stmt->type) {
        ss << " : " << printType(stmt->type);
    }
    if (stmt->initializer) {
        ss << " " << print(stmt->initializer);
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<ClassStmt>& stmt) {
    std::stringstream ss;
    ss << "(class " << stmt->name.lexeme;
    if (!stmt->type_params.empty()) {
        ss << "<";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << stmt->type_params[i].lexeme;
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">";
    }
    for (const auto& member : stmt->members) {
        ss << " " << (member.access == AccessModifier::PUBLIC ? "public" : "private");
        ss << (member.is_static ? " static" : "");
        ss << " " << print(member.declaration);
    }
    ss << ")";
    return ss.str();
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
    ss << "(fun " << stmt->name.lexeme;
    if (!stmt->type_params.empty()) {
        ss << "<";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << stmt->type_params[i].lexeme;
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">";
    }
    ss << " (";
    for (size_t i = 0; i < stmt->params.size(); ++i) {
        ss << stmt->params[i].name.lexeme << ": " << printType(stmt->params[i].type);
        if (i < stmt->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    if (stmt->return_type) {
        ss << ": " << printType(stmt->return_type);
    }
    ss << " " << print(stmt->body);
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

std::any ASTPrinter::visit(const std::shared_ptr<SwitchStmt>& stmt) {
    std::stringstream ss;
    ss << "(switch " << print(stmt->expression);
    for (const auto& case_ : stmt->cases) {
        ss << " (case " << print(case_.condition) << " " << print(case_.body) << ")";
    }
    ss << ")";
    return ss.str();
}

std::any ASTPrinter::visit(const std::shared_ptr<BreakStmt>& stmt) {
    return std::string("(break)");
}

std::any ASTPrinter::visit(const std::shared_ptr<FallthroughStmt>& stmt) {
    return std::string("(fallthrough)");
}

std::any ASTPrinter::visit(const std::shared_ptr<EnumStmt>& stmt) {
    std::stringstream ss;
    ss << "(enum " << stmt->name.lexeme;
    for (const auto& member : stmt->members) {
        ss << " " << member.lexeme;
    }
    ss << ")";
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
