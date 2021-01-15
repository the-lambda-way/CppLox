#pragma once

#include <stdexcept>
#include "Token.h"

class RuntimeError: public std::runtime_error {
public:
  const Token& token;

  RuntimeError(const Token& token, std::string_view message)
    : std::runtime_error{message.data()}, token{token}
  {}
};