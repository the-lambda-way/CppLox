#pragma once

#include <memory>
#include "LoxCallable.h"
#include "Interpreter.h"

// LoxFunction::LoxFunction(std::shared_ptr<Function> declaration)
//   : declaration{std::move(declaration)}
// {}

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration,
                         std::shared_ptr<Environment> closure)
  : declaration{std::move(declaration)}, closure{std::move(closure)}
{}

std::shared_ptr<LoxFunction> LoxInstance::bind(
    std::shared_ptr<LoxInstance> instance) {
  auto environment = std::make_shared<Environment>(closure);
  environment->define("this", instance);
  return std::make_shared<LoxFunction>(declaration, environment);
}

std::string LoxFunction::toString() {
  return "<fn " + declaration->name.lexeme + ">";
}

int LoxFunction::arity() {
  return declaration->params.size();
}

std::any LoxFunction::call(Interpreter& interpreter,
                           std::vector<std::any> arguments) {
  // std::shared_ptr<Environment> environment = interpreter.globals;
  auto environment = std::make_shared<Environment>(closure);
  for (int i = 0; i < declaration->params.size(); ++i) {
    environment->define(declaration->params[i].lexeme,
        arguments[i]);
  }

  // interpreter.executeBlock(declaration->body, environment);
  try {
    interpreter.executeBlock(declaration->body, environment);
  } catch (LoxReturn returnValue) {
    return returnValue.value;
  }
  return nullptr;
}