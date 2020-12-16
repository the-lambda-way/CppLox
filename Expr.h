#pragma once

#include <any>
#include "Token.h"

struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;

struct ExprVisitor {
  virtual std::any visitAssignExpr(Assign* expr) = 0;
  virtual std::any visitBinaryExpr(Binary* expr) = 0;
  virtual std::any visitGroupingExpr(Grouping* expr) = 0;
  virtual std::any visitLiteralExpr(Literal* expr) = 0;
  virtual std::any visitLogicalExpr(Logical* expr) = 0;
  virtual std::any visitUnaryExpr(Unary* expr) = 0;
  virtual std::any visitVariableExpr(Variable* expr) = 0;
};

struct Expr {
  virtual std::any accept(ExprVisitor* visitor) = 0;
  virtual ~Expr () {}
};

struct Assign: Expr {
  Assign(Token name, Expr* value)
    : name{std::move(name)}, value{value}
  {}

  ~Assign() {
    delete value;
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitAssignExpr(this);
  }

  Token const name;
  Expr* const value;
};

struct Binary: Expr {
  Binary(Expr* left, Token op, Expr* right)
    : left{left}, op{std::move(op)}, right{right}
  {}

  ~Binary() {
    delete left;
    delete right;
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitBinaryExpr(this);
  }

  Expr* const left;
  Token const op;
  Expr* const right;
};

struct Grouping: Expr {
  Grouping(Expr* expression)
    : expression{expression}
  {}

  ~Grouping() {
    delete expression;
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitGroupingExpr(this);
  }

  Expr* const expression;
};

struct Literal: Expr {
  Literal(std::any value)
    : value{std::move(value)}
  {}

  ~Literal() {
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitLiteralExpr(this);
  }

  std::any const value;
};

struct Logical: Expr {
  Logical(Expr* left, Token op, Expr* right)
    : left{left}, op{std::move(op)}, right{right}
  {}

  ~Logical() {
    delete left;
    delete right;
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitLogicalExpr(this);
  }

  Expr* const left;
  Token const op;
  Expr* const right;
};

struct Unary: Expr {
  Unary(Token op, Expr* right)
    : op{std::move(op)}, right{right}
  {}

  ~Unary() {
    delete right;
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitUnaryExpr(this);
  }

  Token const op;
  Expr* const right;
};

struct Variable: Expr {
  Variable(Token name)
    : name{std::move(name)}
  {}

  ~Variable() {
  }

  std::any accept(ExprVisitor* visitor) override {
    return visitor->visitVariableExpr(this);
  }

  Token const name;
};

