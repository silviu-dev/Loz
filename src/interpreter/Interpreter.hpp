#pragma once
#include <any>
#include <vector>

#include "Expr.hpp"
#include "IErrorHandler.hpp"

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>
{
  public:
    Interpreter(const std::shared_ptr<IErrorHandler> &errorHandler) : errorHandler_(errorHandler)
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
    void interpret(std::shared_ptr<Expr>);
    void visit(std::shared_ptr<Binary>) override;
    void visit(std::shared_ptr<Grouping>) override;
    void visit(std::shared_ptr<Literal>) override;
    void visit(std::shared_ptr<Unary>) override;
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
    std::any result_ = nullptr;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
};