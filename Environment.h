#pragma once

#include <any>
#include <map>
#include <string_view>
#include "Error.h"
#include "Token.h"

class Environment {
  Environment* const enclosing;
  std::map<std::string_view, std::any> values;

public:
  Environment()
    : enclosing{nullptr}
  {}

  Environment(Environment* enclosing)
    : enclosing{enclosing}
  {}

  std::any get(const Token& name) {
    auto key = values.find(name.lexeme);
    if (key != values.end()) {
      return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name,
        "Undefined variable '" + std::string{name.lexeme} + "'.");
  }

  void assign(const Token& name, const std::any& value) {
    auto key = values.find(name.lexeme);
    if (key != values.end()) {
      values[name.lexeme] = value;
      return;
    }

    if (enclosing != nullptr) {
      enclosing->assign(name, value);
      return;
    }

    throw RuntimeError(name,
        "Undefined variable '" + std::string{name.lexeme} + "'.");
  }

  void define(std::string_view name, const std::any& value) {
    values[name] = value;
  }
};