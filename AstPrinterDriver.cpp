#include <iostream>
#include "AstPrinter.h"
#include "Expr.h"


int main (int argc, char* argv[])
{
     Expr* expression = new Binary
     {
          new Unary
          {
               Token {TokenType::MINUS, "-", empty, 1},
               new Literal {123.}
          },
          Token {TokenType::STAR, "*", empty, 1},
          new Grouping
          {
               new Literal {45.67}
          }
     };

     std::cout << AstPrinter{}.print(expression) << "\n";
}