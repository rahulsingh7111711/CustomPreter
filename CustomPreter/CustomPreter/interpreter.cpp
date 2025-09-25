#include "interpreter.h"
#include <iostream>
#include <stdexcept>

void Interpreter::execute(Block *program) {
    execBlock(program);
}

void Interpreter::execBlock(Block *block) {
    for (auto &stmtPtr : block->statements) {
        execStatement(stmtPtr.get());
    }
}

void Interpreter::execStatement(Statement *st) {
    if (auto p = dynamic_cast<Print*>(st)) {
        int v = evalExpr(p->expr.get());
        std::cout << v << std::endl;
        return;
    }

    if (auto a = dynamic_cast<Assign*>(st)) {
        int v = evalExpr(a->expr.get());
        env.set(a->name, v);
        return;
    }

    if (auto ifs = dynamic_cast<If*>(st)) {
        int cond = evalExpr(ifs->condition.get());
        if (cond != 0) {
            execBlock(ifs->thenBlock.get());
        } else if (ifs->elseBlock) {
            execBlock(ifs->elseBlock.get());
        }
        return;
    }

    if (auto fr = dynamic_cast<For*>(st)) {
        int start = evalExpr(fr->start.get());
        int end = evalExpr(fr->end.get());

        // loop from start to end inclusive, step +1 if start <= end
        if (start <= end) {
            for (int i = start; i <= end; ++i) {
                env.set(fr->varName, i);
                execBlock(fr->block.get());
            }
        } else {
            // If start > end, we'll do nothing (simple behavior).
        }
        return;
    }

    if (auto blk = dynamic_cast<Block*>(st)) {
        execBlock(blk);
        return;
    }

    throw std::runtime_error("Runtime error: unknown statement type");
}

int Interpreter::evalExpr(Expr *e) {
    if (auto n = dynamic_cast<Number*>(e)) {
        return n->value;
    }
    if (auto v = dynamic_cast<Var*>(e)) {
        return env.get(v->name);
    }
    if (auto b = dynamic_cast<BinOp*>(e)) {
        int L = evalExpr(b->left.get());
        int R = evalExpr(b->right.get());
        const std::string &op = b->op;
        if (op == "+") return L + R;
        if (op == "-") return L - R;
        if (op == "*") return L * R;
        if (op == "/") {
            if (R == 0) throw std::runtime_error("Runtime error: division by zero");
            return L / R;
        }
        if (op == "<") return (L < R) ? 1 : 0;
        if (op == ">") return (L > R) ? 1 : 0;
        throw std::runtime_error("Runtime error: unknown binary operator '" + op + "'");
    }
    throw std::runtime_error("Runtime error: unknown expression type");
}
