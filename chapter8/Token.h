#pragma once

#include <any>
#include <string>
#include <utility>      // std::move
#include "TokenType.h"

class Token {
public:
  const TokenType type;
  const std::string lexeme;
  const std::any literal;
  const int line;

  Token(TokenType type, std::string lexeme, std::any literal,
        int line)
    : type{type}, lexeme{std::move(lexeme)},
      literal{std::move(literal)}, line{line}
  {}

  std::string toString() const {
    std::string literal_text;

    switch (type) {
      case (IDENTIFIER):
        literal_text = lexeme;
        break;
      case (STRING):
        literal_text = std::any_cast<std::string>(literal);
        break;
      case (NUMBER):
        literal_text = std::to_string(std::any_cast<double>(literal));
        break;
      case (TRUE):
        literal_text = "true";
        break;
      case (FALSE):
        literal_text = "false";
        break;
      default:
        literal_text = "nil";
    }

    return ::toString(type) + " " + lexeme + " " + literal_text;
  }
};
