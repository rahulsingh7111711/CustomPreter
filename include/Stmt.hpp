
#ifndef STMT_HPP
#define STMT_HPP

#include <memory>
#include <any>
#include "Expr.hpp"

class Stmt {
public:
    struct Visitor {
        virtual std::any visitVarStmt(class Var* stmt) = 0;
        virtual std::any visitExpressionStmt(class Expression* stmt) = 0;
        virtual std::any visitPrintStmt(class Print* stmt) = 0;
        virtual std::any visitIfStmt(class If* stmt) = 0;
        virtual std::any visitBlockStmt(class Block* stmt) = 0;
        virtual std::any visitWhileStmt(class While* stmt) = 0;
        virtual ~Visitor() = default;
    };

    virtual std::any accept(Visitor* visitor) = 0;
    virtual ~Stmt() = default;
};

using StmtPtr = std::shared_ptr<Stmt>;

class Expression : public Stmt {
public:
    ExprPtr expression;

    explicit Expression(ExprPtr expression) : expression(std::move(expression)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitExpressionStmt(this);
    }
};

class Print : public Stmt {
public:
    ExprPtr expression;

    explicit Print(ExprPtr expression) : expression(std::move(expression)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitPrintStmt(this);
    }
};

class Var : public Stmt {
public:
    Token name;
    ExprPtr initializer;

    Var(Token name, ExprPtr initializer) : name(std::move(name)), initializer(std::move(initializer)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitVarStmt(this);
    }
};

class If : public Stmt {
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;

    If(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitIfStmt(this);
    }
};

class Block : public Stmt {
public:
    std::vector<StmtPtr> statements;

    explicit Block(std::vector<StmtPtr> statements)
        : statements(std::move(statements)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitBlockStmt(this);
    }
};

class While : public Stmt {
public:
    ExprPtr condition;
    StmtPtr body;

    While(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::any accept(Visitor* visitor) override {
        return visitor->visitWhileStmt(this);
    }
};


#endif // STMT_HPP
