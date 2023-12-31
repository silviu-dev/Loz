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
    Interpreter(const std::shared_ptr<IErrorHandler> &errorHandler, std::shared_ptr<Environment> newEnv = nullptr)
        : errorHandler_(errorHandler), env_((newEnv != nullptr) ? newEnv : std::make_shared<Environment>())
    {
        if (newEnv == nullptr)
            globals_ = env_;
    }
    struct InterpreterError : public std::exception
    {
        InterpreterError(Token oper, std::string message) : oper(oper), message(message)
        {
        }
        Token oper;
        std::string message;
    };
    struct InterpreterReturn : public std::exception
    {
        InterpreterReturn(std::any result) : result(result)
        {
        }
        std::any result;
    };

    void interpret(std::vector<std::shared_ptr<Stmt>>);
    std::any visit(std::shared_ptr<Class> stmt) override;
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
    std::any visit(std::shared_ptr<Call> expr) override;
    std::any visit(std::shared_ptr<Get> expr) override;
    std::any visit(std::shared_ptr<Set> expr) override;
    std::any visit(std::shared_ptr<Return> ret) override;
    std::any visit(std::shared_ptr<Function> stmt) override;
    std::any visit(std::shared_ptr<Variable>) override;
    std::any visit(std::shared_ptr<This> expr) override;
    std::any visit(std::shared_ptr<Super> super) override;
    std::any lookUpVariable(const Token &name, std::shared_ptr<Expr> expr);
    std::any evaluate(std::shared_ptr<Expr> expr);
    void executeBlock(std::vector<std::shared_ptr<Stmt>> block, std::shared_ptr<Environment> newEnv);
    bool isTruthy(std::any object);
    void checkNumberOperand(Token oper, std::any operand);
    void checkNumberOperands(Token oper, std::any left, std::any right);
    bool isEqual(std::any a, std::any b);
    void resolve(std::shared_ptr<Expr> expr, int depth);

    std::shared_ptr<Environment> globals_ = nullptr;

  private:
    void printValue(std::any value);

    std::any result_ = nullptr;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
    std::shared_ptr<Environment> env_ = nullptr;
    std::map<std::shared_ptr<Expr>, int> locals_;
    friend class FunctionCaller;
};