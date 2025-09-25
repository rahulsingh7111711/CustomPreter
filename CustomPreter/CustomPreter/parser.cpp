#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string &source) : lexer(source) {
    advance();
}

void Parser::advance() {
    current = lexer.getNextToken();
}

bool Parser::match(TokenType type) {
    if (current.type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const std::string &msg) {
    if (current.type != type) {
        throw std::runtime_error("Parse error: expected " + msg + " but got '" + current.text + "'");
    }
    advance();
}

std::unique_ptr<Expr> Parser::parseFactor() {
    // Number
    if (current.type == TokenType::NUMBER) {
        std::string numText = current.text;
        advance();
        return std::make_unique<Number>(std::stoi(numText));
    }

    // Identifier
    if (current.type == TokenType::IDENT) {
        std::string name = current.text;
        advance();
        return std::make_unique<Var>(name);
    }

    // Parenthesized expression
    if (current.type == TokenType::LPAREN) {
        advance(); // consume '('
        auto expr = parseExpr();
        expect(TokenType::RPAREN, ")");
        return expr;
    }

    throw std::runtime_error(std::string("Unexpected token in factor: ") + current.text);
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto left = parseFactor();
    while (current.type == TokenType::MUL || current.type == TokenType::DIV) {
        std::string op = current.text;
        advance();
        auto right = parseFactor();
        left = std::make_unique<BinOp>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseExpr() {
    auto left = parseTerm();
    while (current.type == TokenType::PLUS || current.type == TokenType::MINUS ||
           current.type == TokenType::LT || current.type == TokenType::GT) {
        std::string op = current.text;
        advance();
        auto right = parseTerm();
        left = std::make_unique<BinOp>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Block> Parser::parseBlock() {
    expect(TokenType::LBRACE, "{");
    auto block = std::make_unique<Block>();
    while (current.type != TokenType::RBRACE && current.type != TokenType::END) {
        block->statements.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "}");
    return block;  // ✅ removed std::move
}

std::unique_ptr<Statement> Parser::parseStatement() {
    // print <expr>
    if (current.type == TokenType::PRINT) {
        advance();
        auto expr = parseExpr();
        return std::make_unique<Print>(std::move(expr));
    }

    // assignment: IDENT = expr
    if (current.type == TokenType::IDENT) {
        std::string name = current.text;
        advance();
        expect(TokenType::ASSIGN, "=");
        auto expr = parseExpr();
        return std::make_unique<Assign>(name, std::move(expr));
    }

    // if cond { ... } (else { ... })?
    if (current.type == TokenType::IF) {
        advance();
        auto condition = parseExpr();
        auto thenBlock = parseBlock();
        std::unique_ptr<Block> elseBlock = nullptr;
        if (current.type == TokenType::ELSE) {
            advance();
            elseBlock = parseBlock();
        }
        return std::make_unique<If>(std::move(condition), std::move(thenBlock), std::move(elseBlock));
    }

    // for i = start to end { ... }
    if (current.type == TokenType::FOR) {
        advance();
        if (current.type != TokenType::IDENT) {
            throw std::runtime_error("Expected loop variable name after 'for'");
        }
        std::string varName = current.text;
        advance();

        expect(TokenType::ASSIGN, "=");
        auto startExpr = parseExpr();

        expect(TokenType::TO, "to");
        auto endExpr = parseExpr();

        auto block = parseBlock();
        return std::make_unique<For>(varName, std::move(startExpr), std::move(endExpr), std::move(block));
    }

    throw std::runtime_error(std::string("Unexpected statement: ") + current.text);
}

std::unique_ptr<Block> Parser::parseProgram() {
    auto program = std::make_unique<Block>();
    while (current.type != TokenType::END) {
        program->statements.push_back(parseStatement());
    }
    return program;  // ✅ removed std::move
}
