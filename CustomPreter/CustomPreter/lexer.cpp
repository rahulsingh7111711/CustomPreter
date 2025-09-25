#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string &source) : src(source), pos(0) {}

char Lexer::peek() const {
    return pos < src.size() ? src[pos] : '\0';
}

char Lexer::get() {
    return pos < src.size() ? src[pos++] : '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) get();
}

Token Lexer::getNextToken() {
    skipWhitespace();

    char c = peek();
    if (c == '\0') return {TokenType::END, ""};

    // Numbers
    if (isdigit(c)) {
        std::string num;
        while (isdigit(peek())) num.push_back(get());
        return {TokenType::NUMBER, num};
    }

    // Identifiers / Keywords
    if (isalpha(c)) {
        std::string ident;
        while (isalnum(peek())) ident.push_back(get());

        if (ident == "print") return {TokenType::PRINT, ident};
        if (ident == "if") return {TokenType::IF, ident};
        if (ident == "else") return {TokenType::ELSE, ident};
        if (ident == "for") return {TokenType::FOR, ident};
        if (ident == "to") return {TokenType::TO, ident};

        return {TokenType::IDENT, ident};
    }

    // Symbols
    switch (c) {
        case '+': get(); return {TokenType::PLUS, "+"};
        case '-': get(); return {TokenType::MINUS, "-"};
        case '*': get(); return {TokenType::MUL, "*"};
        case '/': get(); return {TokenType::DIV, "/"};
        case '=': get(); return {TokenType::ASSIGN, "="};
        case '<': get(); return {TokenType::LT, "<"};
        case '>': get(); return {TokenType::GT, ">"};
        case '(': get(); return {TokenType::LPAREN, "("};
        case ')': get(); return {TokenType::RPAREN, ")"};
        case '{': get(); return {TokenType::LBRACE, "{"};
        case '}': get(); return {TokenType::RBRACE, "}"};
    }

    // Unknown character
    get();
    return {TokenType::INVALID, std::string(1, c)};
}
