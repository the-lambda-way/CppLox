#pragma once

#include <any>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include "Expr.h"

// TODO: refer to final Java code to conform the implementation details to jlox, which the book omits from the chapters

class AstPrinter: public ExprVisitor {
public:
  std::string print(Expr* expr) {
    return std::any_cast<std::string>(expr->accept(this));
  }

  std::any visitBinaryExpr(Binary* expr) override {
    return parenthesize(expr->op.lexeme,
                        expr->left, expr->right);
  }

  std::any visitGroupingExpr(Grouping* expr) override {
    return parenthesize("group", expr->expression);
  }

  std::any visitLiteralExpr(Literal* expr) override {
    auto& value_type = expr->value.type();

    if (value_type == typeid(std::string_view)) {
      return std::string{
        std::any_cast<std::string_view>(expr->value)
      };
    } else if (value_type == typeid(double)) {
      return std::to_string(std::any_cast<double>(expr->value));
    } else if (value_type == typeid(bool)) {
      return std::any_cast<bool>(expr->value) ? "true" : "false";
    } else if (value_type == typeid(nullptr)) {
      return "nil";
    }

    return "";
  }

  std::any visitUnaryExpr(Unary* expr) override {
    return parenthesize(expr->op.lexeme, expr->right);
  }

private:
  template <class... E>
  std::string parenthesize(std::string_view name, E*... expr)
  {
    assert((... && std::is_same_v<E, Expr>));

    std::ostringstream builder;

    builder << "(" << name;
    ((builder << " " << print(expr)), ...);
    builder << ")";

    return builder.str();
  }
};