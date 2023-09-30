#pragma once
#include "ICallable.hpp"
#include "Stmt.hpp"

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