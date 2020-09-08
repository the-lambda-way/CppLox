#include <any>
#include <iostream>
#include <sstream>
#include <string>
#include "Expr.h"


// TODO: refer to final Java code to conform the implementation details to jlox, which the book omits from the chapters


// Creates an unambiguous, if ugly, string representation of AST nodes.
class AstPrinter : Visitor
{
public:
     std::string print (const Expr* expr) const
     {
          return std::any_cast<std::string>(expr->accept(this));
     }

     std::any visitBinaryExpr(const Binary* expr) const override
     {
          return parenthesize(expr->op.lexeme, expr->left, expr->right);
     }

     std::any visitGroupingExpr(const Grouping* expr) const override
     {
          return parenthesize("group", expr->expression);
     }

     std::any visitLiteralExpr(const Literal* expr) const override
     {
          // An empty LiteralVal has its own toString overload, so no need to test for it
          return toString(expr->value);
     }

     std::any visitUnaryExpr(const Unary* expr) const override
     {
          return parenthesize(expr->op.lexeme, expr->right);
     }

private:
     template <class... Exprs>
     std::string parenthesize (std::string_view name, const Exprs*... exprs) const
     {
          std::ostringstream builder;

          builder << "(" << name;
          ((builder << " " << print(exprs)), ...);
          builder << ")";

          return builder.str();
     }
};


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
