#pragma once

#include <iostream>
#include <string_view>
#include "Token.h"
#include "TokenType.h"


inline bool hadError = false;


static void report (int line, std::string_view where, std::string_view message)
{
     std::cerr << "[line " << line << "] Error" << where << ": " << message;
     hadError = true;
}


void error (int line, std::string_view message)
{
     report(line, "", message);
}


void error (Token token, std::string_view message)
{
     if (token.type == TokenType::END_OF_FILE)
          report(token.line, " at end", message);
     else
          report(token.line, " at '" + toString(token.lexeme) + "'", message);
}