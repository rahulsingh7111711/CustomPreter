#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "environment.h"
#include <memory>

class Interpreter {
    Environment env;
public:
    Interpreter() = default;
    void execute(Block *program); // execute top-level block
private:
    void execStatement(Statement *st);
    int evalExpr(Expr *e);
    void execBlock(Block *block);
};

#endif
