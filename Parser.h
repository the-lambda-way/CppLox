#pragma once

#include <stdexcept>
#include <utility>         // std::move
#include <vector>
#include "Error.h"
#include "Expr.h"
#include "Token.h"
#include "TokenType.h"

class Parser {
private:
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> tokens;
  int current = 0;

public:
  Parser(std::vector<Token> tokens)
    : tokens{std::move(tokens)}
  {}

  Expr* parse() {
    try {
      return expression();
    } catch (ParseError error) {
      return nullptr;
    }
  }

private:
  Expr* expression() {
    return equality();
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

  Token consume(TokenType type, std::string_view message) {
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

  ParseError error(Token token, std::string_view message) {
    ::error(token, message);
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