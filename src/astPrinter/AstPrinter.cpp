#include <iostream>

#include "AstPrinter.hpp"

using namespace std;

void AstPrinter::print(std::shared_ptr<Expr> expr)
{
    expr->accept(shared_from_this());
}

void AstPrinter::visit(std::shared_ptr<Binary> binary)
{
    parenthesize(binary->oper->lexeme_, {binary->left, binary->right});
}
void AstPrinter::visit(std::shared_ptr<Grouping> grouping)
{
    parenthesize("Grouping", {grouping->expression});
}
void AstPrinter::visit(std::shared_ptr<Literal> literal)
{
    if (literal->value == nullptr)
        cout << "Nil";
    else
    {
        if (*(literal->type) == STRING)
        {
            auto stringPtr = static_pointer_cast<std::string>(literal->value);
            cout << *stringPtr << " ";
        }
        else if (*(literal->type) == NUMBER)
        {
            auto floatPtr = static_pointer_cast<float>(literal->value);

            cout << *floatPtr << " ";
        }
    }
}
void AstPrinter::visit(std::shared_ptr<Unary> unary)
{
    parenthesize(unary->oper->lexeme_, {unary->right});
}
void AstPrinter::parenthesize(std::string Name, std::vector<std::shared_ptr<Expr>> exprVec)
{
    cout << "(" << Name << " ";
    for (auto expr : exprVec)
    {
        expr->accept(shared_from_this());
    }
    cout << ")";
}