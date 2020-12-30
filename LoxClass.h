#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "LoxCallable.h"
#include "Interpreter.h" // LoxFunction declaration

class LoxClass;

class LoxInstance: public std::enable_shared_from_this<LoxInstance> {
  std::shared_ptr<LoxClass> klass;
  std::map<std::string, std::any> fields;

public:
  LoxInstance(std::shared_ptr<LoxClass> klass);
  std::any get(const Token& name);
  void set(const Token& name, std::any value);
  std::string toString();
};

class LoxClass: public LoxCallable,
                public std::enable_shared_from_this<LoxClass> {
  friend class LoxInstance;
  const std::string name;
  std::map<std::string, std::shared_ptr<LoxFunction>> methods;

public:
  // LoxClass(std::string name)
  //   : name{std::move(name)}
  // {}

  LoxClass(std::string name,
      std::map<std::string, std::shared_ptr<LoxFunction>> methods)
    : name{std::move(name)}, methods{std::move(methods)}
  {}

  std::shared_ptr<LoxFunction> findMethod(const std::string& name) {
    auto elem = methods.find(name);
    if (elem != methods.end()) {
      return elem->second;
    }

    return nullptr;
  }

  std::string toString() override {
    return name;
  }

  std::any call(Interpreter& interpreter,
                std::vector<std::any> arguments) override {
    auto instance = std::make_shared<LoxInstance>(shared_from_this());
    return instance;
  }

  int arity() override {
    return 0;
  }
};

// Once again, to keep everything in headers we are sacrificing good
// programming practice.
#include "LoxInstance.h"
