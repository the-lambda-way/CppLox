#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>      // std::move
#include <vector>
#include "Error.h"
#include "Token.h"

class Scanner {
  static const std::map<std::string, TokenType> keywords;

  std::string_view source;
  std::vector<Token> tokens;
  int start = 0;
  int current = 0;
  int line = 1;

public:
  Scanner(std::string_view source)
    : source {source}
  {}

  std::vector<Token> scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    tokens.emplace_back(END_OF_FILE, "", nullptr, line);
    return tokens;
  }

private:
  void scanToken() {
    char c = advance();
    switch (c) {
      case '(': addToken(LEFT_PAREN); break;
      case ')': addToken(RIGHT_PAREN); break;
      case '{': addToken(LEFT_BRACE); break;
      case '}': addToken(RIGHT_BRACE); break;
      case ',': addToken(COMMA); break;
      case '.': addToken(DOT); break;
      case '-': addToken(MINUS); break;
      case '+': addToken(PLUS); break;
      case ';': addToken(SEMICOLON); break;
      case '*': addToken(STAR); break;
      case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
      case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
      case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
      case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;

      case '/':
        if (match('/')) {
          // A comment goes until the end of the line.
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          addToken(SLASH);
        }
        break;

      case ' ':
      case '\r':
      case '\t':
        // Ignore whitespace.
        break;

      case '\n':
        ++line;
        break;

      case '"': string(); break;

      default:
        // error(line, "Unexpected character");

        if (isDigit(c)) {
          number();
        } else if (isAlpha(c)) {
          identifier();
        } else {
          error(line, "Unexpected character.");
        }
        break;
      };
  }

  void identifier() {
    while (isAlphaNumeric(peek())) advance();

    // addToken(IDENTIFIER);

    std::string text =
        std::string{source.substr(start, current - start)};

    TokenType type;
    auto match = keywords.find(text);
    if (match == keywords.end()) {
      type = IDENTIFIER;
    } else {
      type = match->second;
    }

    addToken(type);
  }

  void number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();

      while (isDigit(peek())) advance();
    }

    addToken(NUMBER,
        std::stod(std::string{
            source.substr(start, current - start)}));
  }

  void string() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') ++line;
      advance();
    }

    if (isAtEnd()) {
      error(line, "Unterminated string.");
      return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value{source.substr(start + 1, current - 2 - start)};
    addToken(STRING, value);
  }

  bool match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    ++current;
    return true;
  }

  char peek() {
    if (isAtEnd()) return '\0';
    return source[current];
  }

  char peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
  }

  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
  }

  bool isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
  }

  bool isDigit(char c) {
    return c >= '0' && c <= '9';
  }

  bool isAtEnd() {
    return current >= source.length();
  }

  char advance() {
    return source[current++];
  }

  void addToken(TokenType type) {
    addToken(type, nullptr);
  }

  void addToken(TokenType type, std::any literal) {
    std::string text{source.substr(start, current - start)};
    tokens.emplace_back(type, std::move(text), std::move(literal),
                        line);
  }
};

const std::map<std::string, TokenType> Scanner::keywords =
{
  {"and",    TokenType::AND},
  {"class",  TokenType::CLASS},
  {"else",   TokenType::ELSE},
  {"false",  TokenType::FALSE},
  {"for",    TokenType::FOR},
  {"fun",    TokenType::FUN},
  {"if",     TokenType::IF},
  {"nil",    TokenType::NIL},
  {"or",     TokenType::OR},
  {"print",  TokenType::PRINT},
  {"return", TokenType::RETURN},
  {"super",  TokenType::SUPER},
  {"this",   TokenType::THIS},
  {"true",   TokenType::TRUE},
  {"var",    TokenType::VAR},
  {"while",  TokenType::WHILE},
};
