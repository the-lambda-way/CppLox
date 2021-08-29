#pragma once

#include <any>
#include <memory>
#include <utility>  // std::move
#include <vector>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct ExprVisitor {
  virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
  virtual ~ExprVisitor() = default;
};

struct Expr {
  virtual std::any accept(ExprVisitor& visitor) = 0;
};

struct Binary: Expr, public std::enable_shared_from_this<Binary> {
  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left{std::move(left)}, op{std::move(op)}, right{std::move(right)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitBinaryExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;
};

struct Grouping: Expr, public std::enable_shared_from_this<Grouping> {
  Grouping(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitGroupingExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct Literal: Expr, public std::enable_shared_from_this<Literal> {
  Literal(std::any value)
    : value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitLiteralExpr(shared_from_this());
  }

  const std::any value;
};

struct Unary: Expr, public std::enable_shared_from_this<Unary> {
  Unary(Token op, std::shared_ptr<Expr> right)
    : op{std::move(op)}, right{std::move(right)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitUnaryExpr(shared_from_this());
  }

  const Token op;
  const std::shared_ptr<Expr> right;
};

