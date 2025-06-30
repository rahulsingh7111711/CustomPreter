/*#include <iostream>
#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

int main() {
    std::string source = R"(
        print 1 + 2;
        print (3 + 4) * 5;
        print(6 + 7) * 2; 

    )";

    // Step 1: Tokenize
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // Step 2: Parse
    Parser parser(tokens);
    std::vector<StmtPtr> statements = parser.parse();

    // Step 3: Interpret
    Interpreter interpreter;
    interpreter.interpret(statements);

    return 0;
}
*/

#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include <iostream>
#include <string>

int main() {
    Interpreter interpreter;

    std::cout << "\033[1;32mCustomPreter Interactive Console\033[0m\n";
    std::cout << "Type 'exit;' to quit.\n\n";

    while (true) {
        std::cout << "\033[1;34m>>>\033[0m ";
        std::string line;
        std::getline(std::cin, line);

        if (line == "exit;" || std::cin.eof()) break;

        Scanner scanner(line);
        std::vector<Token> tokens = scanner.scanTokens();

        Parser parser(tokens);
        std::vector<StmtPtr> statements = parser.parse();

        interpreter.interpret(statements);
    }

    return 0;
}
