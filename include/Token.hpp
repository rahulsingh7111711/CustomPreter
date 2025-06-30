#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <any>
#include "TokenType.hpp"

class Token {
public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, const std::string& lexeme, std::any literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string toString() const;
};

#endif // TOKEN_HPP
