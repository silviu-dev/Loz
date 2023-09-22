#include <any>

#include "Interpreter.hpp"

void Interpreter::interpret(std::shared_ptr<Expr> expr)
{
    try
    {
        expr->accept(shared_from_this());
    }
    catch (InterpreterError error)
    {
        errorHandler_->error(error.oper, error.message);
        result_ = nullptr;
    }
}

void Interpreter::visit(std::shared_ptr<Binary> binary)
{
    auto left = evaluate(binary->left);
    auto right = evaluate(binary->right);
    switch (binary->oper.type_)
    {
    case MINUS:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) - std::any_cast<double>(right);
        break;
    case GREATER:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) > std::any_cast<double>(right);
        break;
    case GREATER_EQUAL:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) >= std::any_cast<double>(right);
        break;
    case LESS:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) < std::any_cast<double>(right);
        break;
    case LESS_EQUAL:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) <= std::any_cast<double>(right);
        break;
    case SLASH:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) / std::any_cast<double>(right);
        break;
    case STAR:
        checkNumberOperands(binary->oper, left, right);
        result_ = std::any_cast<double>(left) * std::any_cast<double>(right);
        break;
    case PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            result_ = std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        else if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
        {
            result_ = std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }
        else
        {
            throw InterpreterError(binary->oper, "Operands must be either numbers or strings");
        }
        break;
    case BANG_EQUAL:
        result_ = !isEqual(left, right);
        break;
    case EQUAL_EQUAL:
        result_ = isEqual(left, right);
        break;
    }
}
void Interpreter::visit(std::shared_ptr<Grouping> grouping)
{
    result_ = evaluate(grouping->expression);
}
void Interpreter::visit(std::shared_ptr<Literal> literal)
{
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
        checkNumberOperand(unary->oper, right);
        auto doubleObj = std::any_cast<double>(right);
        result_ = doubleObj * (-1);
    }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    auto interpreter = std::make_shared<Interpreter>(errorHandler_);
    expr->accept(interpreter);
    return interpreter->getResult();
}

bool Interpreter::isTruthy(std::any object)
{
    if (object.type() == typeid(nullptr))
    {
        if (std::any_cast<nullptr_t>(object) == nullptr)
            return false;
    }
    if (object.type() == typeid(bool))
        return std::any_cast<bool>(object);
    return true;
}

void Interpreter::checkNumberOperand(Token oper, std::any operand)
{
    if (operand.type() == typeid(double))
        return;
    throw InterpreterError(oper, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token oper, std::any left, std::any right)
{
    if (left.type() == typeid(double) && right.type() == typeid(double))
        return;
    throw InterpreterError(oper, "Operands must be numbers.");
}

bool Interpreter::isEqual(std::any a, std::any b)
{
    if (a.type() != b.type())
        return false;
    if (a.type() == typeid(double))
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    if (a.type() == typeid(std::string))
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    if (a.type() == typeid(bool))
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
}