#pragma once
#include <any>
#include <iostream>
#include <vector>

#include "Environment.hpp"
#include "IErrorHandler.hpp"
#include "Stmt.hpp"

class Interpreter : public ExprVisitor, public StmtVisitor, public std::enable_shared_from_this<Interpreter>
{
  public:
    Interpreter(const std::shared_ptr<IErrorHandler> &errorHandler, std::shared_ptr<Environment> environment = nullptr)
        : errorHandler_(errorHandler), env_((environment != nullptr) ? environment : std::make_shared<Environment>())
    {
    }
    struct InterpreterError : public std::exception
    {
        InterpreterError(Token oper, std::string message) : oper(oper), message(message)
        {
        }
        Token oper;
        std::string message;
    };
    void interpret(std::vector<std::shared_ptr<Stmt>>);
    std::any visit(std::shared_ptr<Expression> expression) override;
    std::any visit(std::shared_ptr<Print> print) override;
    std::any visit(std::shared_ptr<Var>) override;
    std::any visit(std::shared_ptr<While> stmt) override;
    std::any visit(std::shared_ptr<Block> block) override;
    std::any visit(std::shared_ptr<If> stmt) override;
    std::any visit(std::shared_ptr<Assign> assign) override;
    std::any visit(std::shared_ptr<Logical> expr) override;
    std::any visit(std::shared_ptr<Binary>) override;
    std::any visit(std::shared_ptr<Grouping>) override;
    std::any visit(std::shared_ptr<Literal>) override;
    std::any visit(std::shared_ptr<Unary>) override;
    std::any visit(std::shared_ptr<Variable>) override;
    std::any evaluate(std::shared_ptr<Expr> expr);
    bool isTruthy(std::any object);
    std::any getResult()
    {
        return result_;
    }
    void checkNumberOperand(Token oper, std::any operand);
    void checkNumberOperands(Token oper, std::any left, std::any right);
    bool isEqual(std::any a, std::any b);

  private:
    void printValue(std::any value);

    std::any result_ = nullptr;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
    std::shared_ptr<Environment> env_ = nullptr;
};