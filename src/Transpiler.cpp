#include "Transpiler.hpp"
#include <sstream>
#include <functional>
#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>

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
    return std::string(transpile(expr->left) + " " + expr->op.lexeme + " " + transpile(expr->right));
}

std::any Transpiler::visit(const std::shared_ptr<Grouping>& expr) {
    return std::string("(" + transpile(expr->expression) + ")");
}

std::any Transpiler::visit(const std::shared_ptr<Literal>& expr) {
    if (!expr->value.has_value()) {
        return std::string("nullptr");
    }

    const auto& val = expr->value;
    if (val.type() == typeid(std::string)) {
        return std::string("\"" + std::any_cast<std::string>(val) + "\"");
    }
    if (val.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(val));
    }
    if (val.type() == typeid(double)) {
        std::string str = std::to_string(std::any_cast<double>(val));
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') {
            str += '0';
        }
        return str;
    }
    if (val.type() == typeid(bool)) {
        return std::string(std::any_cast<bool>(val) ? "true" : "false");
    }

    return std::string("unknown literal");
}

std::any Transpiler::visit(const std::shared_ptr<Unary>& expr) {
    switch (expr->op.type) {
        case TokenType::AMPERSAND_AMPERSAND:
            if (auto this_expr = std::dynamic_pointer_cast<ThisExpr>(expr->right)) {
                return std::string("std::move(*this)");
            }
            return std::string(transpile(expr->right) + ".move()");
        case TokenType::STAR:
            if (auto this_expr = std::dynamic_pointer_cast<ThisExpr>(expr->right)) {
                return std::string("this->clone()");
            }
            return std::string(transpile(expr->right) + ".clone()");
        default:
            return std::string(expr->op.lexeme + transpile(expr->right));
    }
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
    return std::string(transpile(expr->target) + " = " + transpile(expr->value));
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
    return std::string(transpile(expr->name) + "[" + transpile(expr->index) + "]");
}

std::any Transpiler::visit(const std::shared_ptr<ScopeExpr>& expr) {
    return std::string(transpile(expr->left) + "::" + expr->name.lexeme);
}

std::any Transpiler::visit(const std::shared_ptr<LambdaExpr>& expr) {
    needs_functional = true;
    std::stringstream ss;
    ss << "[](";
    for (size_t i = 0; i < expr->params.size(); ++i) {
        ss << this->transpileParamType(expr->params[i].type) << " " << expr->params[i].name.lexeme;
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
    if (expr->name.lexeme == "at") {
        return std::string(transpile(expr->object) + ".at");
    }
    if (is_in_static_method) {
        if (auto var_expr = std::dynamic_pointer_cast<Variable>(expr->object)) {
             // Accessing a member through class name (e.g., MyClass.staticMember)
            if (member_static_status.count(expr->name.lexeme) && !member_static_status.at(expr->name.lexeme)) {
                throw std::runtime_error("Cannot access instance member '" + expr->name.lexeme + "' from a static method.");
            }
        }
    } else {
         if (std::dynamic_pointer_cast<ThisExpr>(expr->object)) {
            if (member_static_status.count(expr->name.lexeme) && member_static_status.at(expr->name.lexeme)) {
                // This is an instance method trying to access a static member via `this`
                // This is allowed, but might be better to enforce access via class name
            }
        }
    }
    if (std::dynamic_pointer_cast<ThisExpr>(expr->object)) {
        return std::string(transpile(expr->object) + "->" + expr->name.lexeme);
    }


    return std::string(transpile(expr->object) + "." + expr->name.lexeme);
}

std::any Transpiler::visit(const std::shared_ptr<SetExpr>& expr) {
    if (std::dynamic_pointer_cast<ThisExpr>(expr->object)) {
        if (member_mutability.count(expr->name.lexeme) && !member_mutability[expr->name.lexeme]) {
            throw std::runtime_error("Cannot assign to immutable member '" + expr->name.lexeme + "'.");
        }
        return std::string(transpile(expr->object) + "->" + expr->name.lexeme + " = " + transpile(expr->value));
    }
    if (auto var_expr = std::dynamic_pointer_cast<Variable>(expr->object)) {
        if (member_static_status.count(expr->name.lexeme) && member_static_status.at(expr->name.lexeme)) {
            if (member_mutability.count(expr->name.lexeme) && !member_mutability.at(expr->name.lexeme)) {
                throw std::runtime_error("Cannot assign to immutable static member '" + expr->name.lexeme + "'.");
            }
            return std::string(transpile(expr->object) + "::" + expr->name.lexeme + " = " + transpile(expr->value));
        }
    }
    return std::string(transpile(expr->object) + "." + expr->name.lexeme + " = " + transpile(expr->value));
}

std::any Transpiler::visit(const std::shared_ptr<ThisExpr>& expr) {
    if (is_in_static_method) {
        throw std::runtime_error("Cannot use 'this' in a static method.");
    }
    return std::string("this");
}

std::any Transpiler::visit(const std::shared_ptr<TypeCastExpr>& expr) {
    if (auto grouping = std::dynamic_pointer_cast<Grouping>(expr->expression)) {
        return std::string("static_cast<" + transpileType(expr->type) + ">(" + transpile(grouping->expression) + ")");
    }
    return std::string("static_cast<" + transpileType(expr->type) + ">(" + transpile(expr->expression) + ")");
}

std::any Transpiler::visit(const std::shared_ptr<PrefixExpr>& expr) {
    return std::string(expr->op.lexeme + transpile(expr->right));
}

std::any Transpiler::visit(const std::shared_ptr<PostfixExpr>& expr) {
    return std::string(transpile(expr->left) + expr->op.lexeme);
}

// Statement visitors
std::any Transpiler::visit(const std::shared_ptr<ExpressionStmt>& stmt) {
    return std::string(transpile(stmt->expression) + ";\n");
}

std::string Transpiler::transpileType(const std::shared_ptr<Type>& type) {
    if (!type) return "auto";

    if (auto ref = std::dynamic_pointer_cast<ReferenceType>(type)) {
        switch (ref->kind) {
            case ReferenceKind::MUTABLE:
                return transpileType(ref->referenced_type) + "&";
            case ReferenceKind::IMMUTABLE:
                return "const " + transpileType(ref->referenced_type) + "&";
            case ReferenceKind::MOVE:
                return transpileType(ref->referenced_type) + "&&";
            case ReferenceKind::COPY:
                return transpileType(ref->referenced_type);
        }
    }

    if (type->getKind() == TypeKind::PRIMITIVE) {
        auto primitive = std::dynamic_pointer_cast<PrimitiveType>(type);
        if (primitive->name == "string") {
            return "std::string";
        }
        return primitive->name;
    }
    if (type->getKind() == TypeKind::ARRAY) {
        auto array = std::dynamic_pointer_cast<ArrayType>(type);
        if (array->size.has_value()) {
            required_headers.insert("array");
            return "std::array<" + this->transpileType(array->element_type) + ", " + std::to_string(array->size.value()) + ">";
        }
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
            ss << this->transpileParamType(func->param_types[i]);
            if (i < func->param_types.size() - 1) {
                ss << ", ";
            }
        }
        ss << ")>";
        return ss.str();
    }

    return "auto";
}

std::string Transpiler::transpileParamType(const std::shared_ptr<Type>& type) {
    if (!type) return "auto";

    // If it's an explicit reference type, transpile it as is.
    if (type->getKind() == TypeKind::REFERENCE) {
        return transpileType(type);
    }

    std::string base_type = transpileType(type);
    bool is_complex = false;

    if (type->getKind() == TypeKind::PRIMITIVE) {
        auto primitive = std::dynamic_pointer_cast<PrimitiveType>(type);
        if (primitive->name == "string" || std::all_of(primitive->name.begin(), primitive->name.end(), ::isupper)) {
            is_complex = true;
        }
    } else if (type->getKind() == TypeKind::ARRAY || type->getKind() == TypeKind::FUNCTION) {
        is_complex = true;
    }

    if (is_complex) {
        return "const " + base_type + "&";
    }

    return base_type;
}

std::any Transpiler::visit(const std::shared_ptr<VarStmt>& stmt) {
    std::stringstream ss;
    const std::string qualifier = stmt->is_mutable ? "" : "const ";

    if (stmt->type) {
        ss << qualifier << this->transpileType(stmt->type) << " " << stmt->name.lexeme;
    } else {
        ss << qualifier << "auto " << stmt->name.lexeme;
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
    headers << "#include <string>" << std::endl;
    headers << "#include <vector>" << std::endl;
    if (needs_functional) {
        headers << "#include <functional>" << std::endl;
    }
    for (const auto& header : required_headers) {
        headers << "#include <" << header << ">" << std::endl;
    }
    headers << std::endl;

    return headers.str() + body.str();
}

std::any Transpiler::visit(const std::shared_ptr<FunctionStmt>& stmt) {
    std::stringstream ss;
    if (stmt->is_specialization()) {
        ss << "template <>\n";
    } else if (!stmt->type_params.empty()) {
        ss << "template <";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << "typename " << stmt->type_params[i].name.lexeme;
            if (stmt->type_params[i].default_type) {
                ss << " = " << transpileType(stmt->type_params[i].default_type);
            }
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">\n";
    }

    if (stmt->name.lexeme == "main") {
        ss << "int main(int argc, char* argv[]) ";
        std::stringstream body_ss;
        body_ss << "{\n";
        if (!stmt->params.empty()) {
            if (stmt->params.size() == 1 && stmt->params[0].type->getKind() == TypeKind::ARRAY) {
                body_ss << "std::vector<std::string> " << stmt->params[0].name.lexeme << "(argv, argv + argc);\n";
            } else {
                throw std::runtime_error("Invalid signature for main function. Expected 'fn main(args: string[])'.");
            }
        }
        for (const auto& statement : stmt->body->statements) {
            body_ss << transpile(statement);
        }
        body_ss << "}\n";
        ss << body_ss.str();
    } else {
        ss << transpileType(stmt->return_type) << " ";
        ss << stmt->name.lexeme;
        if (stmt->is_specialization()) {
            ss << "<";
            for (size_t i = 0; i < stmt->specialization_params.size(); ++i) {
                ss << transpileType(stmt->specialization_params[i]);
                if (i < stmt->specialization_params.size() - 1) {
                    ss << ", ";
                }
            }
            ss << ">";
        }
        ss << "(";
        for (size_t i = 0; i < stmt->params.size(); ++i) {
            ss << transpileParamType(stmt->params[i].type) << " " << stmt->params[i].name.lexeme;
            if (i < stmt->params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ") ";
        ss << transpile(stmt->body);
    }
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

std::any Transpiler::visit(const std::shared_ptr<DoWhileStmt>& stmt) {
    std::stringstream ss;
    ss << "do " << transpile(stmt->body) << " while (" << transpile(stmt->condition) << ");\n";
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

std::any Transpiler::visit(const std::shared_ptr<RangeForStmt>& stmt) {
    std::stringstream ss;
    ss << "for (" << (stmt->is_mutable ? "auto& " : "const auto& ") << stmt->variable.lexeme << " : " << transpile(stmt->container) << ") " << transpile(stmt->body);
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<SwitchStmt>& stmt) {
    bool all_cases_constant = true;
    for (const auto& case_stmt : stmt->cases) {
        if (!isConstantExpression(case_stmt.condition)) {
            all_cases_constant = false;
            break;
        }
    }

    std::stringstream ss;
    if (all_cases_constant) {
        ss << "switch (" << transpile(stmt->expression) << ") {\n";
        for (const auto& case_stmt : stmt->cases) {
            ss << "case " << transpile(case_stmt.condition) << ":\n";
            ss << transpile(case_stmt.body);
        }
        ss << "}\n";
    } else {
        std::string expr = transpile(stmt->expression);
        for (size_t i = 0; i < stmt->cases.size(); ++i) {
            if (i == 0) {
                ss << "if (" << expr << " == " << transpile(stmt->cases[i].condition) << ") ";
            } else {
                ss << "else if (" << expr << " == " << transpile(stmt->cases[i].condition) << ") ";
            }
            ss << transpile(stmt->cases[i].body);
        }
    }

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

std::string Transpiler::transpileClassOrStructBody(const Token& name, const std::vector<TypeParameter>& type_params, const std::vector<std::variant<ClassStmt::ClassMember, StructStmt::StructMember>>& members) {
    std::stringstream ss;

    // Pre-scan for const members initialized in constructors
    std::set<std::string> constructor_initialized_consts;
    for (const auto& member_variant : members) {
        std::visit([&](auto&& member){
            using T = std::decay_t<decltype(member)>;
            if constexpr (std::is_same_v<T, ClassStmt::ClassMember>) {
                if (auto func_stmt = std::dynamic_pointer_cast<FunctionStmt>(member.declaration)) {
                    if (func_stmt->name.lexeme == name.lexeme) { // It's a constructor
                        for (const auto& ctor_stmt : func_stmt->body->statements) {
                            if (auto expr_stmt = std::dynamic_pointer_cast<ExpressionStmt>(ctor_stmt)) {
                                if (auto set_expr = std::dynamic_pointer_cast<SetExpr>(expr_stmt->expression)) {
                                    if (std::dynamic_pointer_cast<ThisExpr>(set_expr->object)) {
                                        for (const auto& m_variant : members) {
                                            std::visit([&](auto&& m){
                                                if (auto var_decl = std::dynamic_pointer_cast<VarStmt>(m.declaration)) {
                                                    if (var_decl->name.lexeme == set_expr->name.lexeme && !var_decl->is_mutable) {
                                                        constructor_initialized_consts.insert(set_expr->name.lexeme);
                                                    }
                                                }
                                            }, m_variant);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }, member_variant);
    }

    AccessModifier current_access = AccessModifier::PRIVATE; // C++ default
    bool first_member = true;

    for (const auto& member_variant : members) {
        std::visit([&](auto&& member) {
            using T = std::decay_t<decltype(member)>;
            if constexpr (std::is_same_v<T, ClassStmt::ClassMember>) {
                if (first_member) {
                    current_access = member.access;
                    ss << (current_access == AccessModifier::PUBLIC ? "public:\n" : "private:\n");
                    first_member = false;
                } else if (member.access != current_access) {
                    current_access = member.access;
                    ss << (current_access == AccessModifier::PUBLIC ? "public:\n" : "private:\n");
                }
            }

            std::string static_keyword = member.is_static ? "static " : "";

            if (auto func_stmt = std::dynamic_pointer_cast<FunctionStmt>(member.declaration)) {
                bool was_in_static_method = is_in_static_method;
                is_in_static_method = member.is_static;
                member_static_status[func_stmt->name.lexeme] = member.is_static;
                if constexpr (std::is_same_v<T, ClassStmt::ClassMember>) {
                    member_access_status[func_stmt->name.lexeme] = current_access;
                }

                if constexpr (std::is_same_v<T, ClassStmt::ClassMember>) {
                    // Constructor
                    if (func_stmt->name.lexeme == name.lexeme) {
                        ss << static_keyword << func_stmt->name.lexeme << "(";
                        for (size_t i = 0; i < func_stmt->params.size(); ++i) {
                            ss << transpileParamType(func_stmt->params[i].type) << " " << func_stmt->params[i].name.lexeme;
                            if (i < func_stmt->params.size() - 1) ss << ", ";
                        }
                        ss << ") ";

                        // Generate initializer list for const members
                        std::vector<std::string> initializers;
                        std::vector<std::shared_ptr<Stmt>> remaining_stmts;
                        std::set<std::string> const_members;

                        for (const auto& m_variant_inner : members) {
                            std::visit([&](auto&& m_inner){
                                if (auto var_stmt = std::dynamic_pointer_cast<VarStmt>(m_inner.declaration)) {
                                    if (!var_stmt->is_mutable) {
                                        const_members.insert(var_stmt->name.lexeme);
                                    }
                                }
                            }, m_variant_inner);
                        }

                        for (const auto& ctor_stmt : func_stmt->body->statements) {
                            bool moved = false;
                            if (auto expr_stmt = std::dynamic_pointer_cast<ExpressionStmt>(ctor_stmt)) {
                                if (auto set_expr = std::dynamic_pointer_cast<SetExpr>(expr_stmt->expression)) {
                                    if (std::dynamic_pointer_cast<ThisExpr>(set_expr->object)) {
                                        if (const_members.count(set_expr->name.lexeme)) {
                                            initializers.push_back(set_expr->name.lexeme + "(" + transpile(set_expr->value) + ")");
                                            moved = true;
                                        }
                                    }
                                }
                            }
                            if (!moved) {
                                remaining_stmts.push_back(ctor_stmt);
                            }
                        }

                        if (!initializers.empty()) {
                            ss << ": ";
                            for (size_t i = 0; i < initializers.size(); ++i) {
                                ss << initializers[i];
                                if (i < initializers.size() - 1) ss << ", ";
                            }
                        }
                        auto new_body = std::make_shared<BlockStmt>(remaining_stmts);
                        ss << transpile(new_body);
                    }
                    // Destructor
                    else if (func_stmt->name.lexeme[0] == '~' && func_stmt->name.lexeme.substr(1) == name.lexeme) {
                        ss << static_keyword << func_stmt->name.lexeme << "() " << transpile(func_stmt->body);
                    }
                    // Regular method
                    else {
                        ss << static_keyword << transpileType(func_stmt->return_type) << " " << func_stmt->name.lexeme << "(";
                        for (size_t i = 0; i < func_stmt->params.size(); ++i) {
                            ss << transpileParamType(func_stmt->params[i].type) << " " << func_stmt->params[i].name.lexeme;
                            if (i < func_stmt->params.size() - 1) ss << ", ";
                        }
                        ss << ") " << transpile(func_stmt->body);
                    }
                } else { // Struct
                    ss << static_keyword << transpileType(func_stmt->return_type) << " " << func_stmt->name.lexeme << "(";
                    for (size_t i = 0; i < func_stmt->params.size(); ++i) {
                        ss << transpileParamType(func_stmt->params[i].type) << " " << func_stmt->params[i].name.lexeme;
                        if (i < func_stmt->params.size() - 1) ss << ", ";
                    }
                    ss << ") " << transpile(func_stmt->body);
                }
                is_in_static_method = was_in_static_method;
            } else if (auto var_stmt = std::dynamic_pointer_cast<VarStmt>(member.declaration)) {
                member_mutability[var_stmt->name.lexeme] = var_stmt->is_mutable;
                member_static_status[var_stmt->name.lexeme] = member.is_static;
                if constexpr (std::is_same_v<T, ClassStmt::ClassMember>) {
                    member_access_status[var_stmt->name.lexeme] = current_access;
                }

                ss << (member.is_static ? "inline static " : "") ;
                const std::string qualifier = var_stmt->is_mutable ? "" : "const ";
                ss << qualifier;
                if (var_stmt->type) {
                    ss << transpileType(var_stmt->type) << " " << var_stmt->name.lexeme;
                } else {
                    ss << "auto " << var_stmt->name.lexeme;
                }

                if (var_stmt->initializer) {
                    bool is_const_in_ctor = !var_stmt->is_mutable && constructor_initialized_consts.count(var_stmt->name.lexeme);
                    if (!is_const_in_ctor) {
                        ss << " = " << transpile(var_stmt->initializer);
                    }
                }
                ss << ";\n";
            }
        }, member_variant);
    }
    return ss.str();
}

std::any Transpiler::visit(const std::shared_ptr<ClassStmt>& stmt) {
    std::string previous_class_name = current_class_name;
    current_class_name = stmt->name.lexeme;
    bool was_in_class_body = is_in_class_body;
    is_in_class_body = true;
    member_mutability.clear();
    member_static_status.clear();
    member_access_status.clear();
    std::stringstream ss;
    if (!stmt->type_params.empty()) {
        ss << "template <";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << "typename " << stmt->type_params[i].name.lexeme;
            if (stmt->type_params[i].default_type) {
                ss << " = " << transpileType(stmt->type_params[i].default_type);
            }
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">\n";
    }
    ss << "class " << stmt->name.lexeme << " {\n";

    std::vector<std::variant<ClassStmt::ClassMember, StructStmt::StructMember>> members;
    for(const auto& member : stmt->members) {
        members.push_back(member);
    }

    ss << transpileClassOrStructBody(stmt->name, stmt->type_params, members);

    ss << "};\n";
    is_in_class_body = was_in_class_body;
    current_class_name = previous_class_name;
    return std::any(ss.str());
}

std::any Transpiler::visit(const std::shared_ptr<StructStmt>& stmt) {
    member_mutability.clear();
    member_static_status.clear();
    std::stringstream ss;
    if (!stmt->type_params.empty()) {
        ss << "template <";
        for (size_t i = 0; i < stmt->type_params.size(); ++i) {
            ss << "typename " << stmt->type_params[i].name.lexeme;
            if (stmt->type_params[i].default_type) {
                ss << " = " << transpileType(stmt->type_params[i].default_type);
            }
            if (i < stmt->type_params.size() - 1) {
                ss << ", ";
            }
        }
        ss << ">\n";
    }
    ss << "struct " << stmt->name.lexeme << " {\n";

    std::vector<std::variant<ClassStmt::ClassMember, StructStmt::StructMember>> members;
    for(const auto& member : stmt->members) {
        members.push_back(member);
    }

    ss << transpileClassOrStructBody(stmt->name, stmt->type_params, members);

    ss << "};\n";
    return std::any(ss.str());
}

std::any Transpiler::visit(const std::shared_ptr<ImportStmt>& stmt) {
    if (stmt->is_stdlib) {
        if (stdlib.has_module(stmt->path.lexeme)) {
            auto module = stdlib.get_module(stmt->path.lexeme);
            if (module) {
                for (const auto& header : module->headers) {
                    required_headers.insert(header);
                }
                if (!stmt->symbol.lexeme.empty()) {
                    return module->source + "using " + stmt->path.lexeme + "::" + stmt->symbol.lexeme + ";\n";
                }
                return module->source;
            }
        }
        throw std::runtime_error("Standard library module not found: " + stmt->path.lexeme);
    }

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
        std::cerr << "Error: cannot find or open file '" << full_path << "'." << std::endl;
        throw std::runtime_error("Could not open file: " + full_path);
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();

    std::string old_path = current_path;
    current_path = full_path;

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

    std::string package_name;
    if (!stmts.empty()) {
        if (auto package_stmt = std::dynamic_pointer_cast<PackageStmt>(stmts[0])) {
            package_name = package_stmt->name.lexeme;
        }
    }

    std::string transpiled_code;
    if (!package_name.empty()) {
        std::vector<std::shared_ptr<Stmt>> stmts_without_package(stmts.begin() + 1, stmts.end());
        transpiled_code = transpile(stmts_without_package, false);
    } else {
        transpiled_code = transpile(stmts, false);
    }
    current_path = old_path;


    if (!stmt->symbol.lexeme.empty()) {
        return "using " + module_name + "::" + stmt->symbol.lexeme + ";\n";
    }

    if (!package_name.empty()) {
        return "namespace " + package_name + " {\nnamespace " + module_name + " {\n" + transpiled_code + "}\n}\n";
    }

    return "namespace " + module_name + " {\n" + transpiled_code + "}\n";
}

std::any Transpiler::visit(const std::shared_ptr<PackageStmt>& stmt) {
    return "";
}

bool Transpiler::isConstantExpression(const std::shared_ptr<Expr>& expr) {
    if (!expr) {
        return false;
    }

    if (std::dynamic_pointer_cast<Literal>(expr)) {
        return true;
    }

    if (auto group = std::dynamic_pointer_cast<Grouping>(expr)) {
        return isConstantExpression(group->expression);
    }

    if (auto unary = std::dynamic_pointer_cast<Unary>(expr)) {
        return isConstantExpression(unary->right);
    }

    if (auto binary = std::dynamic_pointer_cast<Binary>(expr)) {
        return isConstantExpression(binary->left) && isConstantExpression(binary->right);
    }

    if (auto scope = std::dynamic_pointer_cast<ScopeExpr>(expr)) {
        // This is a simple heuristic. A more robust implementation might involve
        // symbol table lookups to confirm if the left side is an enum.
        // For now, we'll assume that a ScopeExpr with a Variable on the left
        // is likely an enum access and thus constant.
        if (std::dynamic_pointer_cast<Variable>(scope->left)) {
            return true;
        }
    }

    // Any other expression type (Variable, Call, etc.) is not considered constant.
    return false;
}

}
