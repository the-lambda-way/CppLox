#pragma once

#include <string>


enum class TokenType
{
     // Single-character tokens.
     LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
     COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

     // One or two character tokens.
     BANG, BANG_EQUAL,
     EQUAL, EQUAL_EQUAL,
     GREATER, GREATER_EQUAL,
     LESS, LESS_EQUAL,

     // Literals.
     IDENTIFIER, STRING, NUMBER,

     // Keywords.
     AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
     PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

     END_OF_FILE
};


// For importing TokenType members, remove once gcc implements using enums from C++ 20
namespace TokenTypeMembers
{
     constexpr auto LEFT_PAREN    = TokenType::LEFT_PAREN;
     constexpr auto RIGHT_PAREN   = TokenType::RIGHT_PAREN;
     constexpr auto LEFT_BRACE    = TokenType::LEFT_BRACE;
     constexpr auto RIGHT_BRACE   = TokenType::RIGHT_BRACE;
     constexpr auto COMMA         = TokenType::COMMA;
     constexpr auto DOT           = TokenType::DOT;
     constexpr auto MINUS         = TokenType::MINUS;
     constexpr auto PLUS          = TokenType::PLUS;
     constexpr auto SEMICOLON     = TokenType::SEMICOLON;
     constexpr auto SLASH         = TokenType::SLASH;
     constexpr auto STAR          = TokenType::STAR;
     constexpr auto BANG          = TokenType::BANG;
     constexpr auto BANG_EQUAL    = TokenType::BANG_EQUAL;
     constexpr auto EQUAL         = TokenType::EQUAL;
     constexpr auto EQUAL_EQUAL   = TokenType::EQUAL_EQUAL;
     constexpr auto GREATER       = TokenType::GREATER;
     constexpr auto GREATER_EQUAL = TokenType::GREATER_EQUAL;
     constexpr auto LESS          = TokenType::LESS;
     constexpr auto LESS_EQUAL    = TokenType::LESS_EQUAL;
     constexpr auto IDENTIFIER    = TokenType::IDENTIFIER;
     constexpr auto STRING        = TokenType::STRING;
     constexpr auto NUMBER        = TokenType::NUMBER;
     constexpr auto AND           = TokenType::AND;
     constexpr auto CLASS         = TokenType::CLASS;
     constexpr auto ELSE          = TokenType::ELSE;
     constexpr auto FALSE         = TokenType::FALSE;
     constexpr auto FUN           = TokenType::FUN;
     constexpr auto FOR           = TokenType::FOR;
     constexpr auto IF            = TokenType::IF;
     constexpr auto NIL           = TokenType::NIL;
     constexpr auto OR            = TokenType::OR;
     constexpr auto PRINT         = TokenType::PRINT;
     constexpr auto RETURN        = TokenType::RETURN;
     constexpr auto SUPER         = TokenType::SUPER;
     constexpr auto THIS          = TokenType::THIS;
     constexpr auto TRUE          = TokenType::TRUE;
     constexpr auto VAR           = TokenType::VAR;
     constexpr auto WHILE         = TokenType::WHILE;
     constexpr auto END_OF_FILE   = TokenType::END_OF_FILE;
}


std::string toString (TokenType type)
{
     const std::string strings[] =
     {
          "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
          "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
          "BANG", "BANG_EQUAL",
          "EQUAL", "EQUAL_EQUAL",
          "GREATER", "GREATER_EQUAL",
          "LESS", "LESS_EQUAL",
          "IDENTIFIER", "STRING", "NUMBER",
          "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
          "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",
          "END_OF_FILE"
     };

     return strings[static_cast<int>(type)];
}
