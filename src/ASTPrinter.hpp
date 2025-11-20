#ifndef CHTHOLLY_AST_PRINTER_HPP
#define CHTHOLLY_AST_PRINTER_HPP

#include "AST.hpp"
#include <string>

namespace chtholly {

class ASTPrinter : public ExprVisitor<std::any> {
public:
    std::string print(const Expr& expr);

    std::any visit(const Binary& expr) override;
    std::any visit(const Grouping& expr) override;
    std::any visit(const Literal& expr) override;
    std::any visit(const Unary& expr) override;

private:
    template<typename... Exprs>
    std::string parenthesize(const std::string& name, const Exprs&... exprs);
};

} // namespace chtholly

#endif // CHTHOLLY_AST_PRINTER_HPP
