#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <string>
#include <any>
#include "Token.hpp"

class Expr {
public:
    struct Visitor {
        virtual std::any visitBinaryExpr(class Binary* expr) = 0;
        virtual std::any visitGroupingExpr(class Grouping* expr) = 0;
        virtual std::any visitLiteralExpr(class Literal* expr) = 0;
        virtual std::any visitUnaryExpr(class Unary* expr) = 0;
        virtual std::any visitVariableExpr(class Variable* expr) = 0;
        virtual ~Visitor() = default;
    };

    virtual std::any accept(Visitor* visitor) = 0;
    virtual ~Expr() = default;
};

using ExprPtr = std::shared_ptr<Expr>;

class Binary : public Expr {
public:
    ExprPtr left;
    Token op;
    ExprPtr right;

    Binary(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    ExprPtr expression;

    explicit Grouping(ExprPtr expression)
        : expression(std::move(expression)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    std::any value;

    explicit Literal(std::any value)
        : value(std::move(value)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Token op;
    ExprPtr right;

    Unary(Token op, ExprPtr right)
        : op(op), right(std::move(right)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitUnaryExpr(this);
    }
};

class Variable : public Expr {
public:
    Token name;

    explicit Variable(Token name) : name(std::move(name)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitVariableExpr(this);
    }
};


#endif // EXPR_HPP
