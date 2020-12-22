#pragma once

#include <any>
#include <string_view>
#include "TokenType.h"

class Token
{
public:
  const TokenType type;
  const std::string_view lexeme;
  const std::any literal;
  const int line;

  std::string toString() {
    std::string text;

    switch (type) {
      case (IDENTIFIER):
        text = std::string{lexeme};
        break;
      case (STRING):
        text = std::any_cast<std::string>(literal);
        break;
      case (NUMBER):
        text = std::to_string(std::any_cast<double>(literal));
        break;
      case (TRUE):
        text = "true";
        break;
      case (FALSE):
        text = "false";
        break;
      default:
        text = "nil";
    }

    return ::toString(type) + " " + std::string{lexeme} + " " + text;
  }
};
