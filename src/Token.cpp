#include "Token.hpp"
#include <sstream>

std::string Token::toString() const {
    std::ostringstream out;

    out << "Token(";
    out << "type: " << static_cast<int>(type) << ", ";
    out << "lexeme: \"" << lexeme << "\", ";

    if (literal.has_value()) {
        if (literal.type() == typeid(std::string)) {
            out << "literal: \"" << std::any_cast<std::string>(literal) << "\"";
        } else if (literal.type() == typeid(double)) {
            out << "literal: " << std::any_cast<double>(literal);
        } else {
            out << "literal: [unknown type]";
        }
    } else {
        out << "literal: null";
    }

    out << ", line: " << line << ")";
    return out.str();
}
