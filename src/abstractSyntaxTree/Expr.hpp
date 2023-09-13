class Expr {};

class Binary  : public Expr
{
Binary ( Expr left, Token operator, Expr right)
{
this->Expr = Expr;
this->Token = Token;
this->Expr = Expr;
}
 Expr left;
 Token operator;
 Expr right;
};

class Grouping  : public Expr
{
Grouping ( Expr expression)
{
this->Expr = Expr;
}
 Expr expression;
};

class Literal  : public Expr
{
Literal ( Object value)
{
this->Object = Object;
}
 Object value;
};

class Unary  : public Expr
{
Unary ( Token operator, Expr right)
{
this->Token = Token;
this->Expr = Expr;
}
 Token operator;
 Expr right;
};

