#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "LoxCallable.h"

class Interpreter;
class LoxFunction;

class LoxClass: public LoxCallable,
                public std::enable_shared_from_this<LoxClass> {
  friend class LoxInstance;
  const std::string name;
  const std::shared_ptr<LoxClass> superclass;
  std::map<std::string, std::shared_ptr<LoxFunction>> methods;

public:
  LoxClass(std::string name, std::shared_ptr<LoxClass> superclass,
      std::map<std::string, std::shared_ptr<LoxFunction>> methods);

  std::shared_ptr<LoxFunction> findMethod(const std::string& name);
  std::string toString() override;
  std::any call(Interpreter& interpreter,
                std::vector<std::any> arguments) override;
  int arity() override;
};
