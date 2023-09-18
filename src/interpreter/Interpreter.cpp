#include <any>
#include <iostream>

#include "Interpreter.hpp"

using namespace std;

void Interpreter::interpret(std::shared_ptr<Expr> expr)
{
    expr->accept(shared_from_this());
}

void Interpreter::visit(std::shared_ptr<Binary> binary)
{
    auto left = evaluate(binary->left);
    auto right = evaluate(binary->right);
    switch (binary->oper.type_)
    {
    case MINUS:
        result_ = std::any_cast<double>(left) - std::any_cast<double>(right);
        break;
    case SLASH:
        result_ = std::any_cast<double>(left) / std::any_cast<double>(right);
        break;
    case STAR:
        result_ = std::any_cast<double>(left) * std::any_cast<double>(right);
        break;
    case PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            result_ = std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
        {
            result_ = std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }
        break;
    }
}
void Interpreter::visit(std::shared_ptr<Grouping> grouping)
{
    result_ = evaluate(grouping->expression);
}
void Interpreter::visit(std::shared_ptr<Literal> literal)
{
    std::cout << "visit literal\n";
    result_ = literal->value;
}
void Interpreter::visit(std::shared_ptr<Unary> unary)
{
    auto right = evaluate(unary->right);
    switch (unary->oper.type_)
    {
    case BANG:
        result_ = !isTruthy(right);
        break;
    case MINUS:
        auto doubleObj = std::any_cast<double>(right);
        result_ = doubleObj * (-1);
    }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    auto interpreter = std::make_shared<Interpreter>();
    expr->accept(interpreter);
    return interpreter->getResult();
}

bool Interpreter::isTruthy(std::any object)
{
    if (object.type() == typeid(bool))
        return true;
    return false;
}