#include <any>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct Visitor
{
     virtual std::any visitBinaryExpr(const Binary* expr) const = 0;
     virtual std::any visitGroupingExpr(const Grouping* expr) const = 0;
     virtual std::any visitLiteralExpr(const Literal* expr) const = 0;
     virtual std::any visitUnaryExpr(const Unary* expr) const = 0;
};

struct Expr
{
     virtual std::any accept (const Visitor* visitor) const = 0;
};

struct Binary : Expr
{
     Binary (Expr* left, Token op, Expr* right)
          : left {left}, op {op}, right {right}
     {}

     std::any accept (const Visitor* visitor) const override
     {
          return visitor->visitBinaryExpr(this);
     }

     const Expr* left;
     const Token op;
     const Expr* right;
};

struct Grouping : Expr
{
     Grouping (Expr* expression)
          : expression {expression}
     {}

     std::any accept (const Visitor* visitor) const override
     {
          return visitor->visitGroupingExpr(this);
     }

     const Expr* expression;
};

struct Literal : Expr
{
     Literal (LiteralVal value)
          : value {value}
     {}

     std::any accept (const Visitor* visitor) const override
     {
          return visitor->visitLiteralExpr(this);
     }

     const LiteralVal value;
};

struct Unary : Expr
{
     Unary (Token op, Expr* right)
          : op {op}, right {right}
     {}

     std::any accept (const Visitor* visitor) const override
     {
          return visitor->visitUnaryExpr(this);
     }

     const Token op;
     const Expr* right;
};

