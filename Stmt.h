#pragma once

#include <any>
#include "Token.h"
#include "Expr.h"

struct Block;
struct Expression;
struct Print;
struct Var;

struct StmtVisitor {
  virtual std::any visitBlockStmt(Block* stmt) = 0;
  virtual std::any visitExpressionStmt(Expression* stmt) = 0;
  virtual std::any visitPrintStmt(Print* stmt) = 0;
  virtual std::any visitVarStmt(Var* stmt) = 0;
};

struct Stmt {
  virtual std::any accept(StmtVisitor* visitor) = 0;
  virtual ~Stmt () {}
};

struct Block: Stmt {
  Block(std::vector<Stmt*> statements)
    : statements{std::move(statements)}
  {}

  ~Block() {
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitBlockStmt(this);
  }

  std::vector<Stmt*> const statements;
};

struct Expression: Stmt {
  Expression(Expr* expression)
    : expression{expression}
  {}

  ~Expression() {
    delete expression;
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitExpressionStmt(this);
  }

  Expr* const expression;
};

struct Print: Stmt {
  Print(Expr* expression)
    : expression{expression}
  {}

  ~Print() {
    delete expression;
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitPrintStmt(this);
  }

  Expr* const expression;
};

struct Var: Stmt {
  Var(Token name, Expr* initializer)
    : name{std::move(name)}, initializer{initializer}
  {}

  ~Var() {
    delete initializer;
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitVarStmt(this);
  }

  Token const name;
  Expr* const initializer;
};

