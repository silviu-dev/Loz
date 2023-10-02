#pragma once
#include "Environment.hpp"
#include "ICallable.hpp"
#include "Interpreter.hpp"
#include "Stmt.hpp"

struct FunctionCaller
{
    FunctionCaller(const std::shared_ptr<Function> &declaration, std::shared_ptr<Interpreter> interpreter)
        : declaration_(declaration), interpreter_(interpreter)
    {
        initialEnv_ = interpreter_->env_;
    }
    ~FunctionCaller()
    {
        interpreter_->env_ = initialEnv_;
    }
    void call(std::vector<std::any> arguments)
    {
        auto newEnv = std::make_shared<Environment>(interpreter_->globals_);
        int i = 0;
        for (auto param : declaration_->params)
        {
            newEnv->define(param.lexeme_, arguments[i++]);
        }
        interpreter_->env_ = newEnv;
        interpreter_->interpret(declaration_->body);
    }

  private:
    std::shared_ptr<Function> declaration_;
    std::shared_ptr<Interpreter> interpreter_;
    std::shared_ptr<Environment> initialEnv_;
};

struct RuntimeFunction : public ICallable
{
    RuntimeFunction(const std::shared_ptr<Function> &declaration) : declaration_(declaration)
    {
    }
    std::any call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments);
    int arity();
    std::string toString();

  private:
    std::shared_ptr<Function> declaration_;
};