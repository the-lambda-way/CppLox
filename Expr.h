#pragma once

#include <any>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct Visitor {
  virtual std::any visitBinaryExpr(const Binary* expr) const = 0;
  virtual std::any visitGroupingExpr(const Grouping* expr) const = 0;
  virtual std::any visitLiteralExpr(const Literal* expr) const = 0;
  virtual std::any visitUnaryExpr(const Unary* expr) const = 0;
};

struct Expr {
  virtual std::any accept(const Visitor* visitor) const = 0;
  virtual ~Expr () {}
};

struct Binary : Expr {
  Binary(Expr* left, Token op, Expr* right)
    : left{left}, op{std::move(op)}, right{right}
  {}

  ~Binary() {
    delete left;
    delete right;
  }

  std::any accept(const Visitor* visitor) const override {
    return visitor->visitBinaryExpr(this);
  }

  const Expr* left;
  const Token op;
  const Expr* right;
};

struct Grouping : Expr {
  Grouping(Expr* expression)
    : expression{expression}
  {}

  ~Grouping() {
    delete expression;
  }

  std::any accept(const Visitor* visitor) const override {
    return visitor->visitGroupingExpr(this);
  }

  const Expr* expression;
};

struct Literal : Expr {
  Literal(std::any value)
    : value{std::move(value)}
  {}

  ~Literal() {
  }

  std::any accept(const Visitor* visitor) const override {
    return visitor->visitLiteralExpr(this);
  }

  const std::any value;
};

struct Unary : Expr {
  Unary(Token op, Expr* right)
    : op{std::move(op)}, right{right}
  {}

  ~Unary() {
    delete right;
  }

  std::any accept(const Visitor* visitor) const override {
    return visitor->visitUnaryExpr(this);
  }

  const Token op;
  const Expr* right;
};

