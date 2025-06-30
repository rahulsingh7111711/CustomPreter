#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "Expr.hpp"
#include "Token.hpp"
#include "Stmt.hpp"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    // ✅ New: parses a list of statements
    std::vector<StmtPtr> parse();

private:
    const std::vector<Token>& tokens;
    size_t current = 0;

    // --- Expressions ---
    ExprPtr expression();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr primary();

    // --- Statements ---
    StmtPtr statement();
    StmtPtr printStatement();
    StmtPtr expressionStatement();
    StmtPtr declaration();
    StmtPtr varDeclaration();
    StmtPtr ifStatement();
    StmtPtr block();
    StmtPtr whileStatement();



    // --- Utilities ---
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;

    // error handling (later)
};

#endif // PARSER_HPP
