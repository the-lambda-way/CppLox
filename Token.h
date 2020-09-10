#pragma once

#include <string_view>
#include <variant>
#include "TokenType.h"


// Whereas Java typically uses reference semantics, modern C++ typically favors value semantics. To get value-based
// polymorphic objects in C++, we can use a union or a variant. We could also use heterogeneous templated objects
// instead, but using a homogenous object simplifies the handling of tokens and syntax trees.


using LiteralVal = std::variant<std::monostate,       // nil, valueless tokens
                                std::string_view,     // identifier, string, error
                                double,               // number
                                bool>;                // boolean

auto empty = std::monostate {};


struct Token
{
     const TokenType        type;
     const std::string_view lexeme;
     const LiteralVal       literal;
     const int              line;
};


std::string toString (std::monostate)         { return "nil"; }
std::string toString (std::string_view s)     { return std::string(s); }
std::string toString (double n)               { return std::to_string(n); }
std::string toString (bool b)                 { return b ? "true" : "false"; }

std::string toString (const LiteralVal& v)
{
     return std::visit([] (auto&& arg) { return toString(std::forward<decltype(arg)>(arg)); }, v);
}

std::string toString (const Token& t)
{
     return toString(t.type) + " " + toString(t.lexeme) + " " +  toString(t.literal);
}
