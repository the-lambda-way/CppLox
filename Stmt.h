#pragma once

#include <any>
#include "Token.h"
#include "Expr.h"

struct Block;
struct Expression;
struct If;
struct Print;
struct Var;
struct While;

struct StmtVisitor {
  virtual std::any visitBlockStmt(Block* stmt) = 0;
  virtual std::any visitExpressionStmt(Expression* stmt) = 0;
  virtual std::any visitIfStmt(If* stmt) = 0;
  virtual std::any visitPrintStmt(Print* stmt) = 0;
  virtual std::any visitVarStmt(Var* stmt) = 0;
  virtual std::any visitWhileStmt(While* stmt) = 0;
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

struct If: Stmt {
  If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
    : condition{condition}, thenBranch{thenBranch}, elseBranch{elseBranch}
  {}

  ~If() {
    delete condition;
    delete thenBranch;
    delete elseBranch;
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitIfStmt(this);
  }

  Expr* const condition;
  Stmt* const thenBranch;
  Stmt* const elseBranch;
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

struct While: Stmt {
  While(Expr* condition, Stmt* body)
    : condition{condition}, body{body}
  {}

  ~While() {
    delete condition;
    delete body;
  }

  std::any accept(StmtVisitor* visitor) override {
    return visitor->visitWhileStmt(this);
  }

  Expr* const condition;
  Stmt* const body;
};

