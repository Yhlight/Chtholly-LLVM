#include "Transpiler.hpp"
#include <sstream>
#include <functional>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"

namespace chtholly {

Transpiler::Transpiler(std::string path) : current_path(std::move(path)) {}

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements) {
    return transpile(statements, true);
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
    ss << transpile(expr->callee);
    if (!expr->type_arguments.empty()) {
        ss << "<";
        for (size_t i = 0; i < expr->type_arguments.size(); ++i) {
            ss << transpileType(expr->type_arguments[i]);
            if (i < expr->type_arguments.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">";
    }
    ss << "(";
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

std::any Transpiler::visit(const std::shared_ptr<ScopeExpr>& expr) {
    return transpile(expr->left) + "::" + expr->name.lexeme;
}

std::any Transpiler::visit(const std::shared_ptr<LambdaExpr>& expr) {
    needs_functional = true;
    std::stringstream ss;
    ss << "[](";
    for (size_t i = 0; i < expr->params.size(); ++i) {
        ss << this->transpileType(expr->params[i].type) << " " << expr->params[i].name.lexeme;
        if (i < expr->params.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    if (expr->return_type) {
        ss << " -> " << this->transpileType(expr->return_type);
    }
    ss << " " << transpile(expr->body);
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<GetExpr>& expr) {
    if (std::dynamic_pointer_cast<ThisExpr>(expr->object)) {
        return transpile(expr->object) + "->" + expr->name.lexeme;
    }
    return transpile(expr->object) + "." + expr->name.lexeme;
}

std::any Transpiler::visit(const std::shared_ptr<SetExpr>& expr) {
    if (std::dynamic_pointer_cast<ThisExpr>(expr->object)) {
        return transpile(expr->object) + "->" + expr->name.lexeme + " = " + transpile(expr->value);
    }
    return transpile(expr->object) + "." + expr->name.lexeme + " = " + transpile(expr->value);
}

std::any Transpiler::visit(const std::shared_ptr<ThisExpr>& expr) {
    return std::string("this");
}

// Statement visitors
std::any Transpiler::visit(const std::shared_ptr<ExpressionStmt>& stmt) {
    return transpile(stmt->expression) + ";\n";
}

std::string Transpiler::transpileType(const std::shared_ptr<Type>& type) {
    if (!type) return "auto";
    if (type->getKind() == TypeKind::PRIMITIVE) {
        auto primitive = std::dynamic_pointer_cast<PrimitiveType>(type);
        return primitive->name;
    }
    if (type->getKind() == TypeKind::ARRAY) {
        auto array = std::dynamic_pointer_cast<ArrayType>(type);
        return "std::vector<" + this->transpileType(array->element_type) + ">";
    }
    if (type->getKind() == TypeKind::ENUM) {
        auto enum_type = std::dynamic_pointer_cast<EnumType>(type);
        return enum_type->name;
    }
    if (type->getKind() == TypeKind::FUNCTION) {
        needs_functional = true;
        auto func = std::dynamic_pointer_cast<FunctionType>(type);
        std::stringstream ss;
        ss << "std::function<" << this->transpileType(func->return_type) << "(";
        for (size_t i = 0; i < func->param_types.size(); ++i) {
            ss << this->transpileType(func->param_types[i]);
            if (i < func->param_types.size() - 1) {
                ss << ", ";
            }
        }
        ss << ")>";
        return ss.str();
    }
    return "auto";
}

std::any Transpiler::visit(const std::shared_ptr<VarStmt>& stmt) {
    std::stringstream ss;
    if (stmt->type) {
        ss << this->transpileType(stmt->type) << " " << stmt->name.lexeme;
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

std::string Transpiler::transpile(const std::vector<std::shared_ptr<Stmt>>& statements, bool is_main) {
    std::stringstream body;
    std::string package_name;
    bool has_package = false;
    if (!statements.empty()) {
        if (auto package_stmt = std::dynamic_pointer_cast<PackageStmt>(statements[0])) {
            package_name = package_stmt->name.lexeme;
            has_package = true;
        }
    }

    if (has_package) {
        body << "namespace " << package_name << " {\n";
    }

    for (size_t i = (has_package ? 1 : 0); i < statements.size(); ++i) {
        if (statements[i]) {
            body << transpile(statements[i]);
        }
    }

    if (has_package) {
        body << "}\n";
    }

    if (!is_main) {
        return body.str();
    }

    std::stringstream headers;
    headers << "#include <iostream>" << std::endl;
    headers << "#include <string>" << std::endl;
    headers << "#include <vector>" << std::endl;
    if (needs_functional) {
        headers << "#include <functional>" << std::endl;
    }
    headers << std::endl;

    return headers.str() + body.str();
}

std::any Transpiler::visit(const std::shared_ptr<FunctionStmt>& stmt) {
    std::stringstream ss;
    if (!stmt->type_params.empty()) {
        ss << "template <";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << "typename " << stmt->type_params[i].lexeme;
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">\n";
    }

    if (stmt->name.lexeme == "main") {
        ss << "int main(int argc, char* argv[]) ";
    } else {
        ss << transpileType(stmt->return_type) << " ";
        ss << stmt->name.lexeme << "(";
        for (size_t i = 0; i < stmt->params.size(); ++i) {
            ss << transpileType(stmt->params[i].type) << " " << stmt->params[i].name.lexeme;
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

std::any Transpiler::visit(const std::shared_ptr<EnumStmt>& stmt) {
    std::stringstream ss;
    ss << "enum class " << stmt->name.lexeme << " {\n";
    for (size_t i = 0; i < stmt->members.size(); ++i) {
        ss << "    " << stmt->members[i].lexeme;
        if (i < stmt->members.size() - 1) {
            ss << ",\n";
        }
    }
    ss << "\n};\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<ClassStmt>& stmt) {
    std::stringstream ss;
    if (!stmt->type_params.empty()) {
        ss << "template <";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << "typename " << stmt->type_params[i].lexeme;
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">\n";
    }
    ss << "class " << stmt->name.lexeme << " {\n";

    AccessModifier current_access = AccessModifier::PRIVATE; // C++ default
    bool first_member = true;

    for (const auto& member : stmt->members) {
        if (first_member) {
            current_access = member.access;
            ss << (current_access == AccessModifier::PUBLIC ? "public:\n" : "private:\n");
            first_member = false;
        } else if (member.access != current_access) {
            current_access = member.access;
            ss << (current_access == AccessModifier::PUBLIC ? "public:\n" : "private:\n");
        }

        std::string static_keyword = member.is_static ? "static " : "";

        if (auto func_stmt = std::dynamic_pointer_cast<FunctionStmt>(member.declaration)) {
            // Constructor
            if (func_stmt->name.lexeme == stmt->name.lexeme) {
                ss << static_keyword << func_stmt->name.lexeme << "(";
                 for (size_t i = 0; i < func_stmt->params.size(); ++i) {
                    ss << transpileType(func_stmt->params[i].type) << " " << func_stmt->params[i].name.lexeme;
                    if (i < func_stmt->params.size() - 1) ss << ", ";
                }
                ss << ") " << transpile(func_stmt->body);
            }
            // Destructor
            else if (func_stmt->name.lexeme[0] == '~' && func_stmt->name.lexeme.substr(1) == stmt->name.lexeme) {
                 ss << static_keyword << func_stmt->name.lexeme << "() " << transpile(func_stmt->body);
            }
            // Regular method
            else {
                 ss << static_keyword << transpile(member.declaration);
            }
        } else if (auto var_stmt = std::dynamic_pointer_cast<VarStmt>(member.declaration)) {
            ss << (member.is_static ? "inline static " : "") ;
            if (var_stmt->type) {
                ss << transpileType(var_stmt->type) << " " << var_stmt->name.lexeme;
            } else {
                ss << "auto " << var_stmt->name.lexeme;
            }

            if (var_stmt->initializer) {
                ss << " = " << transpile(var_stmt->initializer);
            }
            ss << ";\n";
        }
    }

    ss << "};\n";
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<ImportStmt>& stmt) {
    std::string path = std::any_cast<std::string>(stmt->path.literal);
    std::string dir = "";
    auto pos = current_path.find_last_of('/');
    if (pos != std::string::npos) {
        dir = current_path.substr(0, pos);
    }

    std::string full_path = dir.empty() ? path : dir + "/" + path;

    if (included_files.count(full_path)) {
        return "";
    }
    included_files.insert(full_path);

    std::ifstream file(full_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + full_path);
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();

    std::string old_path = current_path;
    current_path = full_path;
    std::string transpiled_code = transpile(stmts, false);
    current_path = old_path;

    std::string module_name;
    if (!stmt->alias.lexeme.empty()) {
        module_name = stmt->alias.lexeme;
    } else {
        size_t start = path.find_last_of('/');
        if (start == std::string::npos) {
            start = 0;
        } else {
            start++;
        }

        size_t end = path.find_last_of('.');
        if (end == std::string::npos || end < start) {
            module_name = path.substr(start);
        } else {
            module_name = path.substr(start, end - start);
        }
    }

    return "namespace " + module_name + " {\n" + transpiled_code + "}\n";
}

std::any Transpiler::visit(const std::shared_ptr<PackageStmt>& stmt) {
    return "";
}

}
