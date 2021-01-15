#pragma once

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "Error.h"
#include "Expr.h"
#include "RuntimeError.h"

class Interpreter: public ExprVisitor {
public:
  void interpret(std::shared_ptr<Expr> expression) {
    try {
      std::any value = evaluate(expression);
      std::cout << stringify(value) << "\n";
    } catch (RuntimeError error) {
      runtimeError(error);
    }
  }

private:
  std::any evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(*this);
  }

public:
  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
      case BANG_EQUAL: return !isEqual(left, right);
      case EQUAL_EQUAL: return isEqual(left, right);
      case GREATER:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) >
               std::any_cast<double>(right);
      case GREATER_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) >=
               std::any_cast<double>(right);
      case LESS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) <
               std::any_cast<double>(right);
      case LESS_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) <=
               std::any_cast<double>(right);
      case MINUS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) -
               std::any_cast<double>(right);
      case PLUS:
        if (left.type() == typeid(double) &&
            right.type() == typeid(double)) {
          return std::any_cast<double>(left) +
                 std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string) &&
            right.type() == typeid(std::string)) {
          return std::any_cast<std::string>(left) +
                 std::any_cast<std::string>(right);
        }

        // break;

        throw RuntimeError{expr->op,
            "Operands must be two numbers or two strings."};
      case SLASH:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) /
               std::any_cast<double>(right);
      case STAR:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) *
               std::any_cast<double>(right);
    }

    // Unreachable.
    return {};
  }

  std::any visitGroupingExpr(
      std::shared_ptr<Grouping> expr) override {
    return evaluate(expr->expression);
  }

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override {
    return expr->value;
  }

  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
      case BANG:
        return !isTruthy(right);
      case MINUS:
        checkNumberOperand(expr->op, right);
        return -std::any_cast<double>(right);
    }

    // Unreachable.
    return {};
  }

private:
  void checkNumberOperand(const Token& op,
                          const std::any& operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError{op, "Operand must be a number."};
  }

  void checkNumberOperands(const Token& op,
                           const std::any& left,
                           const std::any& right) {
    if (left.type() == typeid(double) &&
        right.type() == typeid(double)) {
      return;
    }

    throw RuntimeError{op, "Operands must be numbers."};
  }

  bool isTruthy(const std::any& object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object);
    }
    return true;
  }

  bool isEqual(const std::any& a, const std::any& b) {
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
      return true;
    }
    if (a.type() == typeid(nullptr)) return false;

    if (a.type() == typeid(std::string) &&
        b.type() == typeid(std::string)) {
      return std::any_cast<std::string>(a) ==
             std::any_cast<std::string>(b);
    }
    if (a.type() == typeid(double) && b.type() == typeid(double)) {
      return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
      return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
  }

  std::string stringify(const std::any& object) {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(double)) {
      std::string text = std::to_string(
          std::any_cast<double>(object));
      if (text[text.length() - 2] == '.' &&
          text[text.length() - 1] == '0') {
        text = text.substr(0, text.length() - 2);
      }
      return text;
    }

    if (object.type() == typeid(std::string)) {
      return std::any_cast<std::string>(object);
    }
    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object) ? "true" : "false";
    }

    return "Error in stringify: object type not recognized.";
  }
};
