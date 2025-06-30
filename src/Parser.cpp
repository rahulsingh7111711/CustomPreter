#include "Parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}


// ---------------------- Statements ----------------------

StmtPtr Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();  // ✅ Add this
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::LEFT_BRACE})) return block();
    return expressionStatement();
}

StmtPtr Parser::printStatement() {
    ExprPtr value = expression();
    if (!match({TokenType::SEMICOLON})) {
        std::cerr << "Expected ';' after value.\n";
    }
    return std::make_shared<Print>(value);
}

StmtPtr Parser::expressionStatement() {
    ExprPtr expr = expression();
    if (!match({TokenType::SEMICOLON})) {
        std::cerr << "Expected ';' after expression.\n";
    }
    return std::make_shared<Expression>(expr);
}

StmtPtr Parser::declaration() {
    if (match({TokenType::VAR})) return varDeclaration();
    return statement();
}

StmtPtr Parser::varDeclaration() {
    Token name = advance();

    ExprPtr initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    if (!match({TokenType::SEMICOLON})) {
        std::cerr << "Expected ';' after variable declaration.\n";
    }

    return std::make_shared<Var>(name, initializer);
}

StmtPtr Parser::ifStatement() {
    if (!match({TokenType::LEFT_PAREN})) {
        std::cerr << "Expected '(' after 'if'.\n";
    }

    ExprPtr condition = expression();

    if (!match({TokenType::RIGHT_PAREN})) {
        std::cerr << "Expected ')' after condition.\n";
    }

    StmtPtr thenBranch = statement();
    StmtPtr elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_shared<If>(condition, thenBranch, elseBranch);
}

StmtPtr Parser::block() {
    std::vector<StmtPtr> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    if (!match({TokenType::RIGHT_BRACE})) {
        std::cerr << "Expected '}' after block.\n";
    }

    return std::make_shared<Block>(statements);
}

StmtPtr Parser::whileStatement() {
    if (!match({TokenType::LEFT_PAREN})) {
        std::cerr << "Expected '(' after 'while'.\n";
    }

    ExprPtr condition = expression();

    if (!match({TokenType::RIGHT_PAREN})) {
        std::cerr << "Expected ')' after condition.\n";
    }

    StmtPtr body = statement();

    return std::make_shared<While>(condition, body);
}


// ---------------------- Expressions ----------------------

ExprPtr Parser::expression() {
    return equality();
}

ExprPtr Parser::equality() {
    ExprPtr expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        ExprPtr right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::comparison() {
    ExprPtr expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
                  TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        ExprPtr right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::term() {
    ExprPtr expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        ExprPtr right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::factor() {
    ExprPtr expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        ExprPtr right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        ExprPtr right = unary();
        return std::make_shared<Unary>(op, right);
    }

    return primary();
}

ExprPtr Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE}))  return std::make_shared<Literal>(true);
    if (match({TokenType::NIL}))   return std::make_shared<Literal>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }

    if (match({TokenType::LEFT_PAREN})) {
        ExprPtr expr = expression();
        if (!match({TokenType::RIGHT_PAREN})) {
            std::cerr << "Expected ')' after expression.\n";
        }
        return std::make_shared<Grouping>(expr);
    }

    std::cerr << "Unexpected token: " << peek().lexeme << "\n";
    return nullptr;
}


// ---------------------- Utilities ----------------------

bool Parser::match(const std::vector<TokenType>& types) {
    for (const TokenType& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}
