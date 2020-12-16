#pragma once

#include <iostream>
#include <stdexcept>

// Chapter 8 - Statements and State
#include <vector>

#include "Error.h"
#include "Expr.h"

// Chapter 8 - Statements and State
#include "Environment.h"
#include "Stmt.h"

// Chapter 7 - Evaluating Expressions
#include "RuntimeError.h"

// class Interpreter: public ExprVisitor {

// Chapter 8 - Statements and State
class Interpreter: public ExprVisitor,
                   public StmtVisitor {
  // Chapter 8 - Statements and State
  Environment* environment = new Environment{};

public:
  // void interpret(Expr* expression) const {
  //   try {
  //     std::any value = evaluate(expression);
  //     std::cout << stringify(value) << "\n";
  //   } catch (RuntimeError error) {
  //     runtimeError(error);
  //   }
  // }

  // Chapter 8 - Statements and State
  void interpret(std::vector<Stmt*> statements) {
    try {
      for (Stmt* statement : statements) {
        execute(statement);
      }
    } catch (RuntimeError error) {
      runtimeError(error);
    }
  }

private:
  std::any evaluate(Expr* expr) {
    return expr->accept(this);
  }

  // Chapter 8 - Statements and State
  void execute(Stmt* stmt) {
    stmt->accept(this);
  }

public:
  // Chapter 8 - Statements and State
  void executeBlock(std::vector<Stmt*> statements,
                   Environment* environment) {
    Environment* previous = this->environment;
    try {
      this->environment = environment;

      for (Stmt* statement : statements) {
        execute(statement);
      }
    } catch (...) {
      this->environment = previous;
      throw;
    }

    this->environment = previous;
  }

  std::any visitBlockStmt(Block* stmt) override {
    executeBlock(stmt->statements, new Environment{environment});
    return {};
  }

  std::any visitExpressionStmt(Expression* stmt) override {
    evaluate(stmt->expression);
    return {};
  }

  // Chapter 9 - Control Flow
  std::any visitIfStmt(If* stmt) override {
    if (isTruthy(evaluate(stmt->condition))) {
      execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
      execute(stmt->elseBranch);
    }
    return {};
  }

  std::any visitPrintStmt(Print* stmt) override {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << "\n";
    return {};
  }

  // Chapter 8 - Statements and State
  std::any visitVarStmt(Var* stmt) override {
    std::any value = nullptr;
    if (stmt->initializer != nullptr) {
      value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, value);
    return {};
  }

  // Chapter 9 - Control Flow
  std::any visitWhileStmt(While* stmt) override {
    while (isTruthy(evaluate(stmt->condition))) {
      execute(stmt->body);
    }
    return {};
  }

  std::any visitAssignExpr(Assign* expr) override {
    std::any value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return value;
  }

  std::any visitBinaryExpr(Binary* expr) override {
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
      case BANG_EQUAL: return !isEqual(left, right);
      case EQUAL_EQUAL: return isEqual(left, right);
    }

    // Unreachable.
    return nullptr;
  }

  std::any visitGroupingExpr(Grouping* expr) override {
    return evaluate(expr->expression);
  }

  std::any visitLiteralExpr(Literal* expr) override {
    return expr->value;
  }

  // Chapter 9 - Control Flow
  std::any visitLogicalExpr(Logical* expr) override {
    std::any left = evaluate(expr->left);

    if (expr->op.type == OR) {
      if (isTruthy(left)) return left;
    } else {
      if (!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
  }

  std::any visitUnaryExpr(Unary* expr) override {
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

  // Chapter 8 - Statements and State
  std::any visitVariableExpr(Variable* expr) override {
    return environment->get(expr->name);
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

  bool isTruthy(std::any object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object);
    }
    return true;
  }

  bool isEqual(std::any a, std::any b) {
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

  std::string stringify(std::any object) {
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