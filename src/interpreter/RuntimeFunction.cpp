#include "RuntimeFunction.hpp"
#include "Environment.hpp"
#include <iostream>
using namespace std;

std::any RuntimeFunction::call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments)
{
    auto caller = std::make_shared<FunctionCaller>(declaration_, interpreter, closure_);
    caller->call(arguments);
    return nullptr;
}

int RuntimeFunction::arity()
{
    return declaration_->params.size();
}

std::string RuntimeFunction::toString()
{
    return "<fn " + declaration_->name.lexeme_ + ">";
}

ICallablePtr RuntimeFunction::bind(RuntimeClassInstancePtr instance)
{
    auto environment = std::make_shared<Environment>(closure_);
    environment->define("this", instance);
    ICallablePtr var = std::make_shared<RuntimeFunction>(declaration_, environment);
    return var;
}