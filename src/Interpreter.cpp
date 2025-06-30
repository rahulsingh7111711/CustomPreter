#include "Interpreter.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>


std::any Interpreter::interpret(const ExprPtr& expression) {
    try {
        std::any value = expression->accept(this);
        std::cout << stringify(value) << std::endl;
        return value;
    } catch (const std::runtime_error& error) {
        std::cerr << "Runtime error: " << error.what() << std::endl;
        return nullptr;
    }
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements) {
    try {
        for (const StmtPtr& stmt : statements) {
            stmt->accept(this);
        }
    } catch (const std::runtime_error& error) {
        std::cerr << "Runtime error: " << error.what() << std::endl;
    }
}

bool isEqual(const std::any& a, const std::any& b) {
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value() || !b.has_value()) return false;

    if (a.type() != b.type()) return false;

    if (a.type() == typeid(double)) {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }
    if (a.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }

    return false; // unsupported types
}

std::any Interpreter::visitLiteralExpr(Literal* expr) {
    return expr->value;
}

std::any Interpreter::visitGroupingExpr(Grouping* expr) {
    return expr->expression->accept(this);
}

std::any Interpreter::visitUnaryExpr(Unary* expr) {
    std::any right = expr->right->accept(this);

    switch (expr->op.type) {
        case TokenType::MINUS:
            if (right.type() == typeid(double)) {
                return -std::any_cast<double>(right);
            }
            break;
        case TokenType::BANG:
            return !std::any_cast<bool>(right);
        default:
            break;
    }

    throw std::runtime_error("Invalid unary operand.");
}

std::any Interpreter::visitBinaryExpr(Binary* expr) {
    std::any left = expr->left->accept(this);
    std::any right = expr->right->accept(this);

    switch (expr->op.type) {
        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            break;
        case TokenType::MINUS:
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::STAR:
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::SLASH:
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::GREATER:
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::LESS:
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::EQUAL_EQUAL:
            return isEqual(left, right);
        case TokenType::BANG_EQUAL:
            return !isEqual(left, right);

        default:
            break;
    }

    throw std::runtime_error("Invalid binary operands.");
}

std::any Interpreter::visitPrintStmt(Print* stmt) {
    std::any value = stmt->expression->accept(this);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

std::any Interpreter::visitExpressionStmt(Expression* stmt) {
    stmt->expression->accept(this);
    return nullptr;
}

std::string Interpreter::stringify(const std::any& value) {
    if (!value.has_value()) return "nil";
    
    if (value.type() == typeid(bool)) 
        return std::any_cast<bool>(value) ? "true" : "false";
    
    if (value.type() == typeid(double)) {
        double num = std::any_cast<double>(value);
        if (num == static_cast<int>(num))
            return std::to_string(static_cast<int>(num));  // print as int if exact
        return std::to_string(num);
    }

    if (value.type() == typeid(std::string)) 
        return std::any_cast<std::string>(value);

    return "[unknown]";
}

std::any Interpreter::visitVarStmt(Var* stmt) {
    std::any value = nullptr;
    if (stmt->initializer) {
        value = stmt->initializer->accept(this);
    }
    environment->define(stmt->name.lexeme, value);
    return nullptr;
}

std::any Interpreter::visitVariableExpr(Variable* expr) {
    return environment->get(expr->name.lexeme);
}

std::any Interpreter::visitIfStmt(If* stmt) {
    std::any conditionVal = stmt->condition->accept(this);

    bool conditionTrue = false;
    if (conditionVal.type() == typeid(bool)) {
        conditionTrue = std::any_cast<bool>(conditionVal);
    } else if (conditionVal.type() == typeid(double)) {
        conditionTrue = std::any_cast<double>(conditionVal) != 0;
    } else if (conditionVal.type() == typeid(std::nullptr_t)) {
        conditionTrue = false;
    }

    if (conditionTrue) {
        stmt->thenBranch->accept(this);
    } else if (stmt->elseBranch != nullptr) {
        stmt->elseBranch->accept(this);
    }

    return nullptr;
}

std::any Interpreter::visitWhileStmt(While* stmt) {
    while (true) {
        std::any conditionValue = stmt->condition->accept(this);

        bool conditionTrue = false;
        if (conditionValue.type() == typeid(bool)) {
            conditionTrue = std::any_cast<bool>(conditionValue);
        } else if (conditionValue.type() == typeid(double)) {
            conditionTrue = std::any_cast<double>(conditionValue) != 0;
        }

        if (!conditionTrue) break;

        stmt->body->accept(this);
    }

    return nullptr;
}

std::any Interpreter::visitBlockStmt(Block* stmt) {
    std::shared_ptr<Environment> previous = environment;
    environment = std::make_shared<Environment>();

    std::any result = nullptr;
    try {
        for (const auto& statement : stmt->statements) {
            result = statement->accept(this);
        }
    } catch (...) {
        environment = previous;
        throw;
    }

    environment = previous;
    return result;
}

