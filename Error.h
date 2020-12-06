#pragma once

#include <iostream>
#include <string>
#include "RuntimeError.h"
#include "Token.h"

inline bool hadError = false;

// Chapter 7 - Evaluating Expressions
inline bool hadRuntimeError = false;

static void report(int line, std::string_view where, 
                   std::string message) {
  std::cerr <<
      "[line " << line << "] Error" << where << ": " << message <<
      "\n";
  hadError = true;
}

// Chapter 6 - Parsing Expressions
void error(Token token, std::string message) {
  if (token.type == END_OF_FILE) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + std::string{token.lexeme} + "'",
           message);
  }
}

void error(int line, std::string message) {
  report(line, "", message);
}


// Chapter 7 - Evaluating Expressions
void runtimeError(const RuntimeError& error) {
  std::cout << error.what() <<
      "\n[line] " << error.token.line << "]\n";
  hadRuntimeError = true;
}