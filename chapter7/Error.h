#pragma once

#include <iostream>
#include <string_view>
#include "RuntimeError.h"
#include "Token.h"

inline bool hadError = false;
inline bool hadRuntimeError = false;

static void report(int line, std::string_view where,
                   std::string_view message) {
  std::cerr <<
      "[line " << line << "] Error" << where << ": " << message <<
      "\n";
  hadError = true;
}

void error(const Token& token, std::string_view message) {
  if (token.type == END_OF_FILE) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + token.lexeme + "'", message);
  }
}

void error(int line, std::string_view message) {
  report(line, "", message);
}

void runtimeError(const RuntimeError& error) {
  std::cerr << error.what() <<
      "\n[line " << error.token.line << "]\n";
  hadRuntimeError = true;
}
