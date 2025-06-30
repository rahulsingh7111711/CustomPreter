#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "TokenType.hpp"

class Scanner {
public:
    explicit Scanner(const std::string& source);

    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;

    size_t start = 0;
    size_t current = 0;
    int line = 1;

    void scanToken();
    bool isAtEnd() const;
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    void string();
    void number();
    void identifier();

    static std::unordered_map<std::string, TokenType> keywords;
};

#endif // SCANNER_HPP
