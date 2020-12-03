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
    std::string literal_string;

    switch (type) {
      case (IDENTIFIER):
        literal_string = std::string{lexeme};
        break;
      case (STRING):
        literal_string = std::string{std::any_cast<std::string_view>(literal)};
        break;
      case (NUMBER):
        literal_string = std::to_string(std::any_cast<double>(literal));
        break;
      case (TRUE):
        literal_string = "true";
        break;
      case (FALSE):
        literal_string = "false";
        break;
      default:
        literal_string = "nil";
    }

    return ::toString(type) + " " + std::string{lexeme} + " " +  literal_string;
  }
};
