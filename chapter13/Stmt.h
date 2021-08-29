#pragma once

#include <any>
#include <memory>
#include <utility>  // std::move
#include <vector>
#include "Token.h"

#include "Expr.h"

struct Block;
struct Class;
struct Expression;
struct Function;
struct If;
struct Print;
struct Return;
struct Var;
struct While;

struct StmtVisitor {
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitClassStmt(std::shared_ptr<Class> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitReturnStmt(std::shared_ptr<Return> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;
  virtual ~StmtVisitor() = default;
};

struct Stmt {
  virtual std::any accept(StmtVisitor& visitor) = 0;
};

struct Block: Stmt, public std::enable_shared_from_this<Block> {
  Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements{std::move(statements)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitBlockStmt(shared_from_this());
  }

  const std::vector<std::shared_ptr<Stmt>> statements;
};

struct Class: Stmt, public std::enable_shared_from_this<Class> {
  Class(Token name, std::shared_ptr<Variable> superclass, std::vector<std::shared_ptr<Function>> methods)
    : name{std::move(name)}, superclass{std::move(superclass)}, methods{std::move(methods)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitClassStmt(shared_from_this());
  }

  const Token name;
  const std::shared_ptr<Variable> superclass;
  const std::vector<std::shared_ptr<Function>> methods;
};

struct Expression: Stmt, public std::enable_shared_from_this<Expression> {
  Expression(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitExpressionStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct Function: Stmt, public std::enable_shared_from_this<Function> {
  Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body)
    : name{std::move(name)}, params{std::move(params)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitFunctionStmt(shared_from_this());
  }

  const Token name;
  const std::vector<Token> params;
  const std::vector<std::shared_ptr<Stmt>> body;
};

struct If: Stmt, public std::enable_shared_from_this<If> {
  If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
    : condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitIfStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> thenBranch;
  const std::shared_ptr<Stmt> elseBranch;
};

struct Print: Stmt, public std::enable_shared_from_this<Print> {
  Print(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitPrintStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct Return: Stmt, public std::enable_shared_from_this<Return> {
  Return(Token keyword, std::shared_ptr<Expr> value)
    : keyword{std::move(keyword)}, value{std::move(value)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitReturnStmt(shared_from_this());
  }

  const Token keyword;
  const std::shared_ptr<Expr> value;
};

struct Var: Stmt, public std::enable_shared_from_this<Var> {
  Var(Token name, std::shared_ptr<Expr> initializer)
    : name{std::move(name)}, initializer{std::move(initializer)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitVarStmt(shared_from_this());
  }

  const Token name;
  const std::shared_ptr<Expr> initializer;
};

struct While: Stmt, public std::enable_shared_from_this<While> {
  While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition{std::move(condition)}, body{std::move(body)}
  {}

  std::any accept(StmtVisitor& visitor) override {
    return visitor.visitWhileStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;
};

