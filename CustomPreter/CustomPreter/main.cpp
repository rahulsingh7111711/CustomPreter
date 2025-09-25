#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"
#include "interpreter.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source.min>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::ostringstream ss;
    ss << in.rdbuf();
    std::string source = ss.str();

    try {
        Parser parser(source);
        auto program = parser.parseProgram();

        Interpreter interp;
        interp.execute(program.get());
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
