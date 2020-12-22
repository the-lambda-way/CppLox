#pragma once

#include <any>
#include <functional> // less
#include <map>
#include <memory>
#include <string>
#include "Error.h"
#include "Token.h"

class Environment {
  std::shared_ptr<Environment> enclosing;
  std::map<std::string, std::any, std::less<>> values;

public:
  Environment()
    : enclosing{nullptr}
  {}

  Environment(std::shared_ptr<Environment> enclosing)
    : enclosing{std::move(enclosing)}
  {}

  std::any get(const Token& name) {
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
      return elem->second;
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name,
        "Undefined variable '" + std::string{name.lexeme} + "'.");
  }

  void assign(const Token& name, std::any value) {
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
      elem->second = std::move(value);
      return;
    }

    if (enclosing != nullptr) {
      enclosing->assign(name, std::move(value));
      return;
    }

    throw RuntimeError(name,
        "Undefined variable '" + std::string{name.lexeme} + "'.");
  }

  void define(std::string_view name, std::any value) {
    values[std::string{name}] = std::move(value);
  }
};