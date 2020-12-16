#pragma once

#include <stdexcept>
#include <utility>         // std::move
#include <vector>
#include "Error.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"

class Parser {
private:
  struct ParseError: public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token>& tokens;
  int current = 0;

public:
  Parser(const std::vector<Token>& tokens)
    : tokens{tokens}
  {}

  // Expr* parse() {
  //   try {
  //     return expression();
  //   } catch (ParseError error) {
  //     return nullptr;
  //   }
  // }

  // Chapter 8 - Statements and State
  std::vector<Stmt*> parse() {
    std::vector<Stmt*> statements;
    while (!isAtEnd()) {
      // statements.push_back(statement());
      statements.push_back(declaration());
    }

    return statements;
  }

private:
  Expr* expression() {
    // return equality();

    // Chapter 8 - Statements and State
    return assignment();
  }

  // Chapter 8 - Statements and State
  Stmt* declaration() {
    try {
      if (match(VAR)) return varDeclaration();

      return statement();
    } catch (ParseError error) {
      synchronize();
      return nullptr;
    }
  }

  Stmt* statement() {
    // Chapter 9 - Control Flow
    if (match(FOR)) return forStatement();
    if (match(IF)) return ifStatement();

    if (match(PRINT)) return printStatement();

    // Chapter 9 - Control Flow
    if (match(WHILE)) return whileStatement();

    // Chapter 8 - Statements and State
    if (match(LEFT_BRACE)) return new Block{block()};

    return expressionStatement();
  }

  // Chapter 9 - Control Flow
  Stmt* forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer;
    if (match(SEMICOLON)) {
      initializer = nullptr;
    } else if (match(VAR)) {
      initializer = varDeclaration();
    } else {
      initializer = expressionStatement();
    }

    Expr* condition = nullptr;
    if (!check(SEMICOLON)) {
      condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = nullptr;
    if (!check(RIGHT_PAREN)) {
      increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    Stmt* body = statement();

    if (increment != nullptr) {
      body = new Block{{
          body,
          new Expression{increment}}};
    }

    if (condition == nullptr) condition = new Literal{true};
    body = new While{condition, body};

    if (initializer != nullptr) {
      body = new Block{{initializer, body}};
    }

    return body;
  }

  Stmt* ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();
    Stmt* elseBranch = nullptr;
    if (match(ELSE)) {
      elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
  }

  Stmt* printStatement() {
    Expr* value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Print{value};
  }

  // Chapter 8 - Statements and State
  Stmt* varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if (match(EQUAL)) {
      initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var{std::move(name), initializer};
  }

  Stmt* whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    Stmt* body = statement();

    return new While{condition, body};
  }

  Stmt* expressionStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return new Expression{expr};
  }

  // Chapter 8 - Statements and State
  std::vector<Stmt*> block() {
    std::vector<Stmt*> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
  }

  Expr* assignment() {
    // Expr* expr = equality();

    // Chapter 9 - Control Flow
    Expr* expr = orExpression();

    if (match(EQUAL)) {
      Token equals = previous();
      Expr* value = assignment();

      if (Variable* e = dynamic_cast<Variable*>(expr)) {
        Token name = e->name;
        return new Assign{std::move(name), value};
      }

      error(std::move(equals), "Invalid assignment target.");
    }

    return expr;
  }

  // Chapter 9 - Control Flow
  Expr* orExpression() {
    Expr* expr = andExpression();

    while (match(OR)) {
      Token op = previous();
      Expr* right = andExpression();
      expr = new Logical{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* andExpression() {
    Expr* expr = equality();

    while (match(AND)) {
      Token op = previous();
      Expr* right = equality();
      expr = new Logical{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* equality() {
    Expr* expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
      Token op = previous();
      Expr* right = comparison();
      expr = new Binary{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* comparison() {
    Expr* expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
      Token op = previous();
      Expr* right = term();
      expr = new Binary{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* term() {
    Expr* expr = factor();

    while (match(MINUS, PLUS)) {
      Token op = previous();
      Expr* right = factor();
      expr = new Binary{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* factor() {
    Expr* expr = unary();

    while (match(SLASH, STAR)) {
      Token op = previous();
      Expr* right = unary();
      expr = new Binary{expr, std::move(op), right};
    }

    return expr;
  }

  Expr* unary() {
    if (match(BANG, MINUS)) {
      Token op = previous();
      Expr* right = unary();
      return new Unary{std::move(op), right};
    }

    return primary();
  }

  Expr* primary() {
    if (match(FALSE)) return new Literal{false};
    if (match(TRUE)) return new Literal{true};
    if (match(NIL)) return new Literal{nullptr};

    if (match(NUMBER, STRING)) {
      return new Literal{previous().literal};
    }

    // Chapter 8 - Statements and State
    if (match(IDENTIFIER)) {
      return new Variable{previous()};
    }

    if (match(LEFT_PAREN)) {
      Expr* expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return new Grouping{expr};
    }

    throw error(peek(), "Expect expression.");
  }

  template <class... T>
  bool match(T... type) {
    assert((... && std::is_same_v<T, TokenType>));

    if ((... || check(type))) {
      advance();
      return true;
    }

    return false;
  }

  Token consume(TokenType type, std::string message) {
    if (check(type)) return advance();

    throw error(peek(), message);
  }

  bool check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
  }

  Token advance() {
    if (!isAtEnd()) ++current;
    return previous();
  }

  bool isAtEnd() {
    return peek().type == END_OF_FILE;
  }

  Token peek() {
    return tokens[current];
  }

  Token previous() {
    return tokens[current - 1];
  }

  ParseError error(Token token, std::string message) {
    ::error(std::move(token), message);
    return ParseError{""};
  }

  void synchronize() {
    advance();

    while (!isAtEnd()) {
      if (previous().type == SEMICOLON) return;

      switch (peek().type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
        return;
      }

      advance();
    }
  }
};