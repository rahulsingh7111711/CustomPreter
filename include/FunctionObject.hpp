/*
#ifndef FUNCTION_OBJECT_HPP
#define FUNCTION_OBJECT_HPP

#include "Callable.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"
#include "Interpreter.hpp"

class FunctionObject : public Callable {
public:
    Function* declaration;

    explicit FunctionObject(Function* declaration)
        : declaration(declaration) {}

    std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) override {
        auto env = std::make_shared<Environment>(interpreter->environment);
        for (size_t i = 0; i < declaration->params.size(); ++i) {
            env->define(declaration->params[i].lexeme, arguments[i]);
        }

        interpreter->executeBlock(declaration->body, env);
        return nullptr; // support `return` later
    }

    int arity() const override {
        return static_cast<int>(declaration->params.size());
    }

    std::string toString() const override {
        return "<fn " + declaration->name.lexeme + ">";
    }
};

#endif // FUNCTION_OBJECT_HPP
*/