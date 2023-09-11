class Expr {};

silviu Binary  : public Expr
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

silviu Grouping  : public Expr
{
Grouping ( Expr expression)
{
this->Expr = Expr;
}
 Expr expression;
};

silviu Literal  : public Expr
{
Literal ( Object value)
{
this->Object = Object;
}
 Object value;
};

silviu Unary  : public Expr
{
Unary ( Token operator, Expr right)
{
this->Token = Token;
this->Expr = Expr;
}
 Token operator;
 Expr right;
};

