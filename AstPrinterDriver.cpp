#include "AstPrinter.h"

int main(int argc, char* argv[]) {
  Expr* expression = new Binary{
      new Unary{
          Token{MINUS, "-", nullptr, 1},
          new Literal{123.}
      },
      Token{STAR, "*", nullptr, 1},
      new Grouping{
          new Literal{45.67}}};

  std::cout << AstPrinter{}.print(expression) << "\n";
}