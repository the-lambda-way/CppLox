#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>      // std::move
#include <vector>
#include "Error.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"

class Parser {
  struct ParseError: public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token>& tokens;
  int current = 0;

public:
  Parser(const std::vector<Token>& tokens)
    : tokens{tokens}
  {}

  std::vector<std::shared_ptr<Stmt>> parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
      statements.push_back(declaration());
    }

    return statements;
  }

private:
  std::shared_ptr<Expr> expression() {
    return assignment();
  }

  std::shared_ptr<Stmt> declaration() {
    try {
      if (match(CLASS)) return classDeclaration();
      if (match(FUN)) return function("function");
      if (match(VAR)) return varDeclaration();

      return statement();
    } catch (ParseError error) {
      synchronize();
      return nullptr;
    }
  }

  std::shared_ptr<Stmt> classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");
    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::shared_ptr<Function>> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body");

    return std::make_shared<Class>(std::move(name),
                                   std::move(methods));
  }

  std::shared_ptr<Stmt> statement() {
    if (match(FOR)) return forStatement();
    if (match(IF)) return ifStatement();
    if (match(PRINT)) return printStatement();
    if (match(RETURN)) return returnStatement();
    if (match(WHILE)) return whileStatement();
    if (match(LEFT_BRACE)) return std::make_shared<Block>(block());

    return expressionStatement();
  }

  std::shared_ptr<Stmt> forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match(SEMICOLON)) {
      initializer = nullptr;
    } else if (match(VAR)) {
      initializer = varDeclaration();
    } else {
      initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON)) {
      condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
      increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    std::shared_ptr<Stmt> body = statement();

    if (increment != nullptr) {
      body = std::make_shared<Block>(
          std::vector<std::shared_ptr<Stmt>>{
              body,
              std::make_shared<Expression>(increment)});
    }

    if (condition == nullptr) {
      condition = std::make_shared<Literal>(true);
    }
    body = std::make_shared<While>(condition, body);

    if (initializer != nullptr) {
      body = std::make_shared<Block>(
          std::vector<std::shared_ptr<Stmt>>{initializer, body});
    }

    return body;
  }

  std::shared_ptr<Stmt> ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(ELSE)) {
      elseBranch = statement();
    }

    return std::make_shared<If>(condition, thenBranch, elseBranch);
  }

  std::shared_ptr<Stmt> printStatement() {
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(value);
  }

  std::shared_ptr<Stmt> returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(SEMICOLON)) {
      value = expression();
    }

    consume(SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<Return>(keyword, value);
  }

  std::shared_ptr<Stmt> varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match(EQUAL)) {
      initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(std::move(name), initializer);
  }

  std::shared_ptr<Stmt> whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(condition, body);
  }

  std::shared_ptr<Stmt> expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
  }

  std::shared_ptr<Function> function(std::string kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }

        parameters.push_back(
            consume(IDENTIFIER, "Expect parameter name."));
      } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();
    return std::make_shared<Function>(std::move(name),
                                      std::move(parameters),
                                      std::move(body));
  }

  std::vector<std::shared_ptr<Stmt>> block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
  }

  std::shared_ptr<Expr> assignment() {
    std::shared_ptr<Expr> expr = orExpression();

    if (match(EQUAL)) {
      Token equals = previous();
      std::shared_ptr<Expr> value = assignment();

      if (Variable* e = dynamic_cast<Variable*>(expr.get())) {
        Token name = e->name;
        return std::make_shared<Assign>(std::move(name), value);
      } else if (Get* get = dynamic_cast<Get*>(expr.get())) {
        return std::make_shared<Set>(get->object, get->name, value);
      }

      error(std::move(equals), "Invalid assignment target.");
    }

    return expr;
  }

  std::shared_ptr<Expr> orExpression() {
    std::shared_ptr<Expr> expr = andExpression();

    while (match(OR)) {
      Token op = previous();
      std::shared_ptr<Expr> right = andExpression();
      expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> andExpression() {
    std::shared_ptr<Expr> expr = equality();

    while (match(AND)) {
      Token op = previous();
      std::shared_ptr<Expr> right = equality();
      expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> equality() {
    std::shared_ptr<Expr> expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
      Token op = previous();
      std::shared_ptr<Expr> right = comparison();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> comparison() {
    std::shared_ptr<Expr> expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
      Token op = previous();
      std::shared_ptr<Expr> right = term();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> term() {
    std::shared_ptr<Expr> expr = factor();

    while (match(MINUS, PLUS)) {
      Token op = previous();
      std::shared_ptr<Expr> right = factor();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> factor() {
    std::shared_ptr<Expr> expr = unary();

    while (match(SLASH, STAR)) {
      Token op = previous();
      std::shared_ptr<Expr> right = unary();
      expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
  }

  std::shared_ptr<Expr> unary() {
    if (match(BANG, MINUS)) {
      Token op = previous();
      std::shared_ptr<Expr> right = unary();
      return std::make_shared<Unary>(std::move(op), right);
    }

    return call();
  }

  std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Can't have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN,
                          "Expect ')' after arguments.");

    return std::make_shared<Call>(callee,
                                  std::move(paren),
                                  std::move(arguments));
  }

  std::shared_ptr<Expr> call() {
    std::shared_ptr<Expr> expr = primary();

    while (true) {
      if (match(LEFT_PAREN)) {
        expr = finishCall(expr);
      } else if (match(DOT)) {
        Token name = consume(IDENTIFIER,
            "Expect property name after '.'.");
        expr = std::make_shared<Get>(expr, std::move(name));
      } else {
        break;
      }
    }

    return expr;
  }

  std::shared_ptr<Expr> primary() {
    if (match(FALSE)) return std::make_shared<Literal>(false);
    if (match(TRUE)) return std::make_shared<Literal>(true);
    if (match(NIL)) return std::make_shared<Literal>(nullptr);

    if (match(NUMBER, STRING)) {
      return std::make_shared<Literal>(previous().literal);
    }

    if (match(THIS)) return std::make_shared<This>(previous());

    if (match(IDENTIFIER)) {
      return std::make_shared<Variable>(previous());
    }

    if (match(LEFT_PAREN)) {
      std::shared_ptr<Expr> expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_shared<Grouping>(expr);
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
    return tokens.at(current);
  }

  Token previous() {
    return tokens.at(current - 1);
  }

  ParseError error(const Token& token, std::string_view message) {
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