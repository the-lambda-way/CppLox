#pragma once

#include <string_view>
#include <variant>
#include "TokenType.h"


// Whereas Java typically uses reference semantics, modern C++ typically favors value semantics. To get value-based
// polymorphic objects in C++, we can use a union or a variant. We could also use heterogeneous templated objects
// instead, but using a homogenous object simplifies the handling of tokens.


using TokenVal = std::variant<std::monostate,       // valueless tokens
                              std::string_view,     // identifier, string, error
                              double>;              // number

auto empty = std::monostate {};


struct Token
{
     const TokenType        type;
     const std::string_view lexeme;
     const TokenVal         literal;
     const int              line;
};


std::string to_string (std::monostate)         { return "empty"; }
std::string to_string (std::string_view s)     { return {s.data(), s.length()}; }
std::string to_string (double n)               { return std::to_string(n); }

std::string to_string (const TokenVal& v)
{
     return std::visit([] (auto&& arg) { return to_string(std::forward<decltype(arg)>(arg)); }, v);
}

std::string to_string (const Token& t)
{
     return to_string(t.type) + " " + to_string(t.lexeme) + " " +  to_string(t.literal);
}
