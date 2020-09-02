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


std::string toString (std::monostate)         { return "empty"; }
std::string toString (std::string_view s)     { return {s.data(), s.length()}; }
std::string toString (double n)               { return std::to_string(n); }

std::string toString (const TokenVal& v)
{
     return std::visit([] (auto&& arg) { return toString(std::forward<decltype(arg)>(arg)); }, v);
}

std::string toString (const Token& t)
{
     return toString(t.type) + " " + toString(t.lexeme) + " " +  toString(t.literal);
}
