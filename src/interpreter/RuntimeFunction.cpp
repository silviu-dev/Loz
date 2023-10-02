#include "RuntimeFunction.hpp"
#include <iostream>
using namespace std;

std::any RuntimeFunction::call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments)
{
    auto caller = std::make_shared<FunctionCaller>(declaration_, interpreter);
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
