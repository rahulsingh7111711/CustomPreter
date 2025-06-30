#include "AstPrinter.hpp"
#include <sstream>

std::string AstPrinter::print(const ExprPtr& expr) {
    return std::any_cast<std::string>(expr->accept(this));
}

std::any AstPrinter::visitBinaryExpr(Binary* expr) {
    return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

std::any AstPrinter::visitGroupingExpr(Grouping* expr) {
    return parenthesize("group", {expr->expression});
}

std::any AstPrinter::visitLiteralExpr(Literal* expr) {
    if (!expr->value.has_value()) return "nil";
    if (expr->value.type() == typeid(std::string))
        return std::any_cast<std::string>(expr->value);
    if (expr->value.type() == typeid(double)) {
        std::ostringstream out;
        out << std::any_cast<double>(expr->value);
        return out.str();
    }
    return "[unknown]";
}

std::any AstPrinter::visitUnaryExpr(Unary* expr) {
    return parenthesize(expr->op.lexeme, {expr->right});
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<ExprPtr>& exprs) {
    std::ostringstream out;
    out << "(" << name;
    for (const auto& expr : exprs) {
        out << " " << print(expr);
    }
    out << ")";
    return out.str();
}
