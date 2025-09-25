#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class Expr : public ASTNode {};

class Number : public Expr {
public:
    int value;
    Number(int v) : value(v) {}
};

class Var : public Expr {
public:
    std::string name;
    Var(const std::string &n) : name(n) {}
};

class BinOp : public Expr {
public:
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;

    BinOp(std::unique_ptr<Expr> l, const std::string &o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
};

class Statement : public ASTNode {};

class Assign : public Statement {
public:
    std::string name;
    std::unique_ptr<Expr> expr;

    Assign(const std::string &n, std::unique_ptr<Expr> e)
        : name(n), expr(std::move(e)) {}
};

class Print : public Statement {
public:
    std::unique_ptr<Expr> expr;
    Print(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};

class Block : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
};

class If : public Statement {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Block> thenBlock;
    std::unique_ptr<Block> elseBlock;

    If(std::unique_ptr<Expr> cond, std::unique_ptr<Block> thenB, std::unique_ptr<Block> elseB = nullptr)
        : condition(std::move(cond)), thenBlock(std::move(thenB)), elseBlock(std::move(elseB)) {}
};

class For : public Statement {
public:
    std::string varName;
    std::unique_ptr<Expr> start;
    std::unique_ptr<Expr> end;
    std::unique_ptr<Block> block;

    For(const std::string &v, std::unique_ptr<Expr> s, std::unique_ptr<Expr> e, std::unique_ptr<Block> b)
        : varName(v), start(std::move(s)), end(std::move(e)), block(std::move(b)) {}
};

#endif
