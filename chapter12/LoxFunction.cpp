#include "LoxFunction.h"
#include <utility>        // std::move
#include "Environment.h"
#include "LoxInstance.h"
#include "Interpreter.h"
#include "Stmt.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration,
                         std::shared_ptr<Environment> closure,
                         bool isInitializer)
  : isInitializer{isInitializer}, closure{std::move(closure)},
    declaration{std::move(declaration)}
{}

std::shared_ptr<LoxFunction> LoxFunction::bind(
    std::shared_ptr<LoxInstance> instance) {
  auto environment = std::make_shared<Environment>(closure);
  environment->define("this", instance);
  // return std::make_shared<LoxFunction>(declaration, environment);
  return std::make_shared<LoxFunction>(declaration, environment,
                                       isInitializer);
}

std::string LoxFunction::toString() {
  return "<fn " + declaration->name.lexeme + ">";
}

int LoxFunction::arity() {
  return declaration->params.size();
}

std::any LoxFunction::call(Interpreter& interpreter,
                           std::vector<std::any> arguments) {
  auto environment = std::make_shared<Environment>(closure);
  for (int i = 0; i < declaration->params.size(); ++i) {
    environment->define(declaration->params[i].lexeme,
        arguments[i]);
  }

  try {
    interpreter.executeBlock(declaration->body, environment);
  } catch (LoxReturn returnValue) {
    if (isInitializer) return closure->getAt(0, "this");

    return returnValue.value;
  }

  if (isInitializer) return closure->getAt(0, "this");
  return nullptr;
}
