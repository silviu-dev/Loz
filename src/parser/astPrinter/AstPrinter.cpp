#include <iostream>

#include "AstPrinter.hpp"

using namespace std;

void AstPrinter::print(std::shared_ptr<Stmt> stmt)
{
    stmt->accept(shared_from_this());
    cout << ";";
}

void AstPrinter::visit(std::shared_ptr<Expression> expression)
{
    expression->expression->accept(shared_from_this());
}

void AstPrinter::visit(std::shared_ptr<Print> print)
{
    cout << "print ";
    print->expression->accept(shared_from_this());
}

void AstPrinter::visit(std::shared_ptr<Var> variable)
{
    parenthesize(variable->name.lexeme_, {variable->initializer});
}

void AstPrinter::visit(std::shared_ptr<Binary> binary)
{
    parenthesize(binary->oper.lexeme_, {binary->left, binary->right});
}
void AstPrinter::visit(std::shared_ptr<Grouping> grouping)
{
    parenthesize("Grouping", {grouping->expression});
}
void AstPrinter::visit(std::shared_ptr<Literal> literal)
{
    if (literal->value.type() == typeid(void *) && std::any_cast<void *>(literal->value) == nullptr)
        cout << "Nil";
    else
    {
        if (literal->value.type() == typeid(double))
        {
            auto stringObj = std::any_cast<double>(literal->value);
            cout << stringObj << " ";
        }
        else if (literal->value.type() == typeid(std::string))
        {
            auto doubleObj = std::any_cast<std::string>(literal->value);
            cout << doubleObj << " ";
        }
        if (literal->value.type() == typeid(bool))
        {
            auto boolObj = std::any_cast<bool>(literal->value);
            cout << boolObj;
        }
    }
}
void AstPrinter::visit(std::shared_ptr<Unary> unary)
{
    parenthesize(unary->oper.lexeme_, {unary->right});
}

void AstPrinter::visit(std::shared_ptr<Variable> variable)
{
    cout << "(" << variable->name.lexeme_ << ")";
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