#pragma once

#include <iostream>
#include <stdexcept>
#include "Error.h"
#include "Expr.h"
#include "RuntimeError.h"

class Interpreter : public Visitor {
public:
  void interpret(Expr* expression) const {
    try {
      std::any value = evaluate(expression);
      std::cout << stringify(value) << "\n";
    } catch (RuntimeError error) {
      runtimeError(error);
    }
  }

private:
  std::any evaluate(const Expr* expr) const {
    return expr->accept(this);
  }

public:
  std::any visitBinaryExpr(const Binary* expr) const {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
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
        checkNumberOperands(expr->op, left, right);
        if (left.type() == typeid(double) &&
            right.type() == typeid(double)) {
          return std::any_cast<double>(left) +
                 std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string_view) &&
            right.type() == typeid(std::string_view)) {
          return std::string{std::any_cast<std::string_view>(left)} +
                 std::string{std::any_cast<std::string_view>(right)};
        }

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
      case BANG_EQUAL: return !isEqual(left, right);
      case EQUAL_EQUAL: return isEqual(left, right);
    }

    // Unreachable.
    return nullptr;
  }

  std::any visitGroupingExpr(const Grouping* expr) const {
    return evaluate(expr->expression);
  }

  std::any visitLiteralExpr(const Literal* expr) const {
    return expr->value;
  }

  std::any visitUnaryExpr(const Unary* expr) const {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
      case BANG:
        return !isTruthy(right);
      case MINUS:
        checkNumberOperand(expr->op, right);
        return -std::any_cast<double>(right);
    }

    // Unreachable.
    return nullptr;
  }

private:
  void checkNumberOperand(const Token& op, std::any operand) const {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError{op, "Operand must be a number."};
  }

  void checkNumberOperands(const Token& op,
                           std::any left, std::any right) const {
    if (left.type() == typeid(double) &&
        right.type() == typeid(double)) {
      return;
    }

    throw RuntimeError{op, "Operands must be numbers."};
  }

  bool isTruthy(std::any object) const {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object);
    }
    return true;
  }

  bool isEqual(std::any a, std::any b) const {
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
      return true;
    }
    if (a.type() == typeid(nullptr)) return false;

    if (a.type() == typeid(std::string_view) &&
        b.type() == typeid(std::string_view)) {
      return std::any_cast<std::string_view>(a) ==
             std::any_cast<std::string_view>(b);
    }
    if (a.type() == typeid(double) && b.type() == typeid(double)) {
      return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
      return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
  }

  std::string stringify(std::any object) const {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(double)) {
      std::string text = std::to_string(std::any_cast<double>(object));
      if (text[text.length() - 1] == '0' &&
          text[text.length() - 2] == '.') {
        text = text.substr(0, text.length() - 2);
      }
      return text;
    }

    if (object.type() == typeid(std::string_view)) {
      return std::string{std::any_cast<std::string_view>(object)};
    }

    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object) ? "true" : "false";
    }

    return "";
  }
};