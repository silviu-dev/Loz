#pragma once
#include "Interpreter.hpp"
#include <any>
#include <memory>
#include <vector>

struct ICallable
{
    virtual std::any call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments) = 0;
    virtual int arity() = 0;
};

using ICallablePtr = std::shared_ptr<ICallable>;