#pragma once

#include <any>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct Visitor {
  virtual std::any visitBinaryExpr(const Binary* expr) = 0;
  virtual std::any visitGroupingExpr(const Grouping* expr) = 0;
  virtual std::any visitLiteralExpr(const Literal* expr) = 0;
  virtual std::any visitUnaryExpr(const Unary* expr) = 0;
};

struct Expr {
  virtual std::any accept(Visitor* visitor) const = 0;
  virtual ~Expr () {}
};

struct Binary : Expr {
  Binary(Expr* left, Token op, Expr* right)
    : left {left}, op {op}, right {right}
  {}

  ~Binary() {
    delete left;
    delete right;
  }

  std::any accept(Visitor* visitor) const override {
    return visitor->visitBinaryExpr(this);
  }

  const Expr* left;
  const Token op;
  const Expr* right;
};

struct Grouping : Expr {
  Grouping(Expr* expression)
    : expression {expression}
  {}

  ~Grouping() {
    delete expression;
  }

  std::any accept(Visitor* visitor) const override {
    return visitor->visitGroupingExpr(this);
  }

  const Expr* expression;
};

struct Literal : Expr {
  Literal(std::any value)
    : value {value}
  {}

  ~Literal() {
  }

  std::any accept(Visitor* visitor) const override {
    return visitor->visitLiteralExpr(this);
  }

  const std::any value;
};

struct Unary : Expr {
  Unary(Token op, Expr* right)
    : op {op}, right {right}
  {}

  ~Unary() {
    delete right;
  }

  std::any accept(Visitor* visitor) const override {
    return visitor->visitUnaryExpr(this);
  }

  const Token op;
  const Expr* right;
};

