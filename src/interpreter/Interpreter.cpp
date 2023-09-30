#include <any>
#include <chrono>
#include <iostream>
#include <string>

#include "ICallable.hpp"
#include "Interpreter.hpp"
#include "RuntimeFunction.hpp"
using namespace std;
class ClockNativeFunction : public ICallable
{
    std::any call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments) override
    {
        return (double)(std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count());
    }
    int arity() override
    {
        return 0;
    }
    std::string toString()
    {
        return "<native fn>";
    }
};

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> stmtVec)
{
    // ICallablePtr clockPtr = std::make_shared<ClockNativeFunction>();
    // globals_->define("clock", clockPtr);

    try
    {
        for (auto stmt : stmtVec)
        {
            stmt->accept(shared_from_this());
        }
    }
    catch (InterpreterError error)
    {
        errorHandler_->error(error.oper, error.message);
    }
    catch (RunError error)
    {
        errorHandler_->error(error.oper, error.message);
    }
}

std::any Interpreter::visit(std::shared_ptr<Expression> expression)
{
    evaluate(expression->expression);
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Print> print)
{
    auto val = evaluate(print->expression);
    printValue(val);
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Var> declaration)
{
    std::any value = nullptr;
    if (declaration->initializer != nullptr)
    {
        value = evaluate(declaration->initializer);
    }
    env_->define(declaration->name.lexeme_, value);
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<While> stmt)
{
    while (isTruthy(evaluate(stmt->condition)))
    {
        stmt->body->accept(shared_from_this());
    }
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Block> block)
{
    auto newEnv = std::make_shared<Environment>(env_);
    executeBlock(block->statements, newEnv);
    return nullptr;
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> block, std::shared_ptr<Environment> newEnv)
{
    auto oldEnv = env_;
    env_ = newEnv;
    interpret(block);
    env_ = oldEnv;
}

std::any Interpreter::visit(std::shared_ptr<If> stmt)
{
    if (isTruthy(evaluate(stmt->condition)))
    {
        stmt->thenBranch->accept(shared_from_this());
    }
    else if (stmt->elseBranch != nullptr)
    {
        stmt->elseBranch->accept(shared_from_this());
    }
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Assign> assign)
{
    auto val = evaluate(assign->value);
    env_->assign(assign->name, val);
    return val;
}

std::any Interpreter::visit(std::shared_ptr<Logical> expr)
{
    auto left = evaluate(expr->left);
    if (expr->oper.type_ == OR)
    {
        if (isTruthy(left))
            return left;
    }
    else
    {
        if (!isTruthy(left))
            return left;
    }
    return evaluate(expr->right);
}

std::any Interpreter::visit(std::shared_ptr<Binary> binary)
{
    auto left = evaluate(binary->left);
    auto right = evaluate(binary->right);
    switch (binary->oper.type_)
    {
    case MINUS:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
        break;
    case GREATER:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
        break;
    case GREATER_EQUAL:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
        break;
    case LESS:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
        break;
    case LESS_EQUAL:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
        break;
    case SLASH:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);
        break;
    case STAR:
        checkNumberOperands(binary->oper, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
        break;
    case PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        else if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }
        else
        {
            throw InterpreterError(binary->oper, "Operands must be either numbers or strings");
        }
        break;
    case BANG_EQUAL:
        return !isEqual(left, right);
        break;
    case EQUAL_EQUAL:
        return isEqual(left, right);
        break;
    }
}
std::any Interpreter::visit(std::shared_ptr<Grouping> grouping)
{
    return evaluate(grouping->expression);
}
std::any Interpreter::visit(std::shared_ptr<Literal> literal)
{
    return literal->value;
}

std::any Interpreter::visit(std::shared_ptr<Call> expr)
{
    auto callee = evaluate(expr->callee);
    std::vector<std::any> arguments{};
    for (auto argument : expr->arguments)
    {
        arguments.push_back(evaluate(argument));
    }
    ICallablePtr function;
    try
    {
        function = std::any_cast<ICallablePtr>(callee);
    }
    catch (...)
    {
        throw InterpreterError(expr->paren, "Can only call functions and classes.");
    }

    if (arguments.size() != function->arity())
    {
        throw InterpreterError(expr->paren, std::string("Expected ") + std::to_string(function->arity()) +
                                                " arguments but got " + std::to_string(arguments.size()) + ".");
    }

    try
    {
        return function->call(shared_from_this(), arguments);
    }
    catch (InterpreterReturn returner)
    {
        env_ = env_->enclosing;
        return returner.result;
    }
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Return> ret)
{
    std::any value = nullptr;
    if (ret->value != nullptr)
    {
        value = evaluate(ret->value);
    }
    throw InterpreterReturn(value);
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Function> stmt) // function declaration
{
    ICallablePtr function = std::make_shared<RuntimeFunction>(stmt);
    env_->define(stmt->name.lexeme_, function);
    return nullptr;
}

std::any Interpreter::visit(std::shared_ptr<Unary> unary)
{
    auto right = evaluate(unary->right);
    switch (unary->oper.type_)
    {
    case BANG:
        return !isTruthy(right);
        break;
    case MINUS:
        checkNumberOperand(unary->oper, right);
        auto doubleObj = std::any_cast<double>(right);
        return doubleObj * (-1);
    }
}

std::any Interpreter::visit(std::shared_ptr<Variable> variable)
{
    return env_->get(variable->name);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    return expr->accept(shared_from_this());
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
    {
        std::cout << std::any_cast<double>(value) << "\n";
    }
    if (value.type() == typeid(std::string))
    {
        std::cout << std::any_cast<std::string>(value) << "\n";
    }
    if (value.type() == typeid(bool))
    {
        std::cout << std::any_cast<bool>(value) << "\n";
    }
}