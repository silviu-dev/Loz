#include <any>

#include "Interpreter.hpp"

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> expr)
{
    try
    {
        for (auto e : expr)
            e->accept(shared_from_this());
    }
    catch (InterpreterError error)
    {
        errorHandler_->error(error.oper, error.message);
        result_ = nullptr;
    }
}

void Interpreter::visit(std::shared_ptr<Expression> expression)
{
    auto val = evaluate(expression->expression);
    printValue(val);
}

void Interpreter::visit(std::shared_ptr<Print> print)
{
    auto val = evaluate(print->expression);
    printValue(val);
}

void Interpreter::visit(std::shared_ptr<Var> variable)
{
    std::any value = nullptr;
    if (variable->initializer != nullptr)
    {
        value = evaluate(variable->initializer);
    }
    env_->define(variable->name.lexeme_, value);
}

void Interpreter::visit(std::shared_ptr<Assign> assign)
{
    std::cout << "assign new value "
              << "\n";
    printValue(evaluate(assign->value));
    std::cout << " to the variable: " << assign->name.lexeme_ << "<<\n";

    env_->assign(assign->name, evaluate(assign->value));
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

void Interpreter::visit(std::shared_ptr<Variable> variable)
{
    result_ = env_->get(variable->name);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    expr->accept(shared_from_this());
    return result_;
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

void Interpreter::printValue(std::any value)
{
    if (value.type() == typeid(double))
        std::cout << std::any_cast<double>(value) << "\n";
    if (value.type() == typeid(std::string))
        std::cout << std::any_cast<std::string>(value) << "\n";
    if (value.type() == typeid(bool))
        std::cout << std::any_cast<bool>(value) << "\n";
}