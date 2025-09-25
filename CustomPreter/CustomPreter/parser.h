#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <memory>
#include <vector>

class Parser {
    Lexer lexer;
    Token current;

    void advance();
    bool match(TokenType type);
    void expect(TokenType type, const std::string &msg);

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Block> parseBlock();

public:
    Parser(const std::string &source);
    std::unique_ptr<Block> parseProgram();
};

#endif
