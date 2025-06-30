#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <vector>           // ✅ for std::vector
#include "Expr.hpp"         // ✅ for ExprPtr

class AstPrinter : public Expr::Visitor {
public:
    std::string print(const ExprPtr& expr);

    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;

private:
    std::string parenthesize(const std::string& name, const std::vector<ExprPtr>& exprs);
};

#endif // AST_PRINTER_HPP
