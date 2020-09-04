#include <vector>
#include "Token.h"

class Expr
{
};

struct Binary : Expr
{
     Expr left;
     Token operator;
     Expr right;
};

struct Grouping : Expr
{
     Expr expression;
};

struct Literal : Expr
{
     Object value;
};

struct Unary : Expr
{
     Token operator;
     Expr right;
};

