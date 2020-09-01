#pragma once

#include <map>
#include <string_view>
#include <vector>
#include "Token.h"
#include "TokenType.h"


// Located in Lox.cpp
extern void error (int, std::string_view);


class Scanner
{
public:
     Scanner (std::string_view source);
     const std::vector<Token>& scanTokens ();


private:
     std::string_view source;
     std::vector<Token> tokens;

     int start   = 0;
     int current = 0;
     int line    = 1;

     const std::map<std::string_view, TokenType> keywords;

     // Tokenization
     bool isAlpha (char c);
     bool isAlphaNumeric (char c);
     bool isDigit (char c);
     void identifier ();
     void number ();
     void string ();

     // Traversal
     char peek ();
     char peekNext ();
     char advance ();
     bool isAtEnd ();
     bool match (char expected);

     // Execute
     void scanToken ();
     void addToken (TokenType type);

     template <typename ValueType>
     void addToken (TokenType type, ValueType literal);
};


Scanner::Scanner (std::string_view source) :
     source {source},
     keywords({
          {"and",    TokenType::AND},
          {"class",  TokenType::CLASS},
          {"else",   TokenType::ELSE},
          {"false",  TokenType::FALSE},
          {"for",    TokenType::FOR},
          {"fun",    TokenType::FUN},
          {"if",     TokenType::IF},
          {"nil",    TokenType::NIL},
          {"or",     TokenType::OR},
          {"print",  TokenType::PRINT},
          {"return", TokenType::RETURN},
          {"super",  TokenType::SUPER},
          {"this",   TokenType::THIS},
          {"true",   TokenType::TRUE},
          {"var",    TokenType::VAR},
          {"while",  TokenType::WHILE},
     })
{}


const std::vector<Token>& Scanner::scanTokens ()
{
     while (!isAtEnd())
     {
          // We are at the beginning of the next lexeme.
          start = current;
          scanToken();
     }

     tokens.emplace_back(TokenType::END_OF_FILE, "", empty, line);
     return tokens;
}


void Scanner::scanToken ()
{
     char c = advance();

     switch (c)
     {
          using namespace TokenTypeMembers;

          case '(' :     addToken(LEFT_PAREN);  break;
          case ')' :     addToken(RIGHT_PAREN); break;
          case '{' :     addToken(LEFT_BRACE);  break;
          case '}' :     addToken(RIGHT_BRACE); break;
          case ',' :     addToken(COMMA);       break;
          case '.' :     addToken(DOT);         break;
          case '-' :     addToken(MINUS);       break;
          case '+' :     addToken(PLUS);        break;
          case ';' :     addToken(SEMICOLON);   break;
          case '*' :     addToken(STAR);        break;
          case '!' :     addToken(match('=') ? BANG_EQUAL    : BANG);    break;
          case '=' :     addToken(match('=') ? EQUAL_EQUAL   : EQUAL);   break;
          case '<' :     addToken(match('=') ? LESS_EQUAL    : LESS);    break;
          case '>' :     addToken(match('=') ? GREATER_EQUAL : GREATER); break;

          case '/':
               if (match('/'))
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd())     advance();
               else
                    addToken(SLASH);
               break;

          case ' ':
          case '\r':
          case '\t':
               // Ignore whitespace.
               break;

          case '\n':
               ++line;
               break;

          case '"':     string(); break;

          default:
               if      (isDigit(c))     number();
               else if (isAlpha(c))     identifier();
               else                     error(line, "Unexpected character.");
               break;
    };
}


void Scanner::identifier ()
{
     while (isAlphaNumeric(peek()))     advance();

     // See if the identifier is a reserved word.
     std::string_view text = source.substr(start, current - start);

     TokenType type;
     auto match = keywords.find(text);

     if (match == keywords.end())     type = TokenType::IDENTIFIER;
     else                             type = match->second;

     addToken(type);
}


void Scanner::number ()
{
     while (isDigit(peek()))     advance();

     // Look for a fractional part.
     if (peek() == '.' && isDigit(peekNext()))
     {
          // Consume the "."
          advance();

          while (isDigit(peek()))     advance();
     }

     addToken(TokenType::NUMBER, std::stod(to_string(source.substr(start, current - start))));
}


void Scanner::string ()
{
     while (peek() != '"' && !isAtEnd())
     {
          if (peek() == '\n')     ++line;
          advance();
     }

     // Unterminated string.
     if (isAtEnd())
     {
          error(line, "Unterminated string.");
          return;
     }

     // The closing ".
     advance();

     // Trim the surrounding quotes.
     std::string_view value = source.substr(start + 1, current - 2 - start);
     addToken(TokenType::STRING, value);
}


bool Scanner::match (char expected)
{
     if (isAtEnd()) return false;
     if (source[current] != expected)     return false;

     ++current;
     return true;
}


char Scanner::peek ()
{
     if (isAtEnd())     return '\0';
     return source[current];
}


char Scanner::peekNext ()
{
     if (current + 1 >= source.length())     return '\0';
     return source[current + 1];
}


bool Scanner::isAlpha (char c)
{
     return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
             c == '_';
}


bool Scanner::isAlphaNumeric (char c)
{
     return isAlpha(c) || isDigit(c);
}


bool Scanner::isDigit (char c)
{
     return c >= '0' && c <= '9';
}


bool Scanner::isAtEnd ()
{
     return current >= source.length();
}


char Scanner::advance ()
{
     return source[current++];
}


void Scanner::addToken (TokenType type)
{
     addToken(type, empty);
}


void Scanner::addToken (TokenType type, auto literal)
{
     std::string_view text = source.substr(start, current - start);
     tokens.emplace_back(type, text, literal, line);
}
