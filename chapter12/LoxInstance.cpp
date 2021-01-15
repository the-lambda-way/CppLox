#include "LoxInstance.h"
#include <utility>        // std::move
// #include "Error.h"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass)
  : klass{std::move(klass)}
{}

std::any LoxInstance::get(const Token& name) {
  auto elem = fields.find(name.lexeme);
  if (elem != fields.end()) {
    return elem->second;
  }

  std::shared_ptr<LoxFunction> method =
      klass->findMethod(name.lexeme);
  // if (method != nullptr) return method;
  if (method != nullptr) return method->bind(shared_from_this());

  throw RuntimeError(name,
      "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(const Token& name, std::any value) {
  fields[name.lexeme] = std::move(value);
}

std::string LoxInstance::toString() {
  return klass->name + " instance";
}
