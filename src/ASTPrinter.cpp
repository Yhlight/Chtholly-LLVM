#include "ASTPrinter.hpp"
#include <sstream>
#include <string>

namespace chtholly {

std::string ASTPrinter::print(const Expr& expr) {
    return std::any_cast<std::string>(expr.accept(*this));
}

std::any ASTPrinter::visit(const Binary& expr) {
    return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::any ASTPrinter::visit(const Grouping& expr) {
    return parenthesize("group", *expr.expression);
}

std::any ASTPrinter::visit(const Literal& expr) {
    if (!expr.value.has_value()) {
        return std::string("nil");
    }

    const auto& val = expr.value;
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

std::any ASTPrinter::visit(const Unary& expr) {
    return parenthesize(expr.op.lexeme, *expr.right);
}

template<typename... Exprs>
std::string ASTPrinter::parenthesize(const std::string& name, const Exprs&... exprs) {
    std::stringstream ss;
    ss << "(" << name;
    ((ss << " " << print(exprs)), ...);
    ss << ")";
    return ss.str();
}

} // namespace chtholly
