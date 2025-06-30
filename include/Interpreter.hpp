#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <any>
#include <vector>
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"

class Interpreter : public Expr::Visitor, public Stmt::Visitor {
public:
    // Interpret a list of statements
    void interpret(const std::vector<StmtPtr>& statements);

    // Evaluate a single expression
    std::any interpret(const ExprPtr& expression);

    // Expression visitors
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitVariableExpr(Variable* expr) override;

    // Statement visitors
    std::any visitBlockStmt(Block* stmt) override;
    std::any visitWhileStmt(While* stmt) override;
    std::any visitIfStmt(If* stmt) override;
    std::any visitPrintStmt(Print* stmt) override;
    std::any visitExpressionStmt(Expression* stmt) override;
    std::any visitVarStmt(Var* stmt) override;

private:
    std::shared_ptr<Environment> environment;
    std::string stringify(const std::any& value);
};

#endif // INTERPRETER_HPP
