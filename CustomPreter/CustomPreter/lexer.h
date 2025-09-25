#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    NUMBER, IDENT,
    PRINT, IF, ELSE, FOR, TO,
    PLUS, MINUS, MUL, DIV,
    ASSIGN, LT, GT,
    LPAREN, RPAREN, LBRACE, RBRACE,
    END,      // End of input
    INVALID
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
    std::string src;
    size_t pos;
public:
    Lexer(const std::string &source);
    Token getNextToken();
    void skipWhitespace();
    char peek() const;
    char get();
};

#endif
