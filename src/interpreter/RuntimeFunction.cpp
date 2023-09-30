#include "RuntimeFunction.hpp"
#include "Environment.hpp"
#include <iostream>
using namespace std;

std::any RuntimeFunction::call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments)
{
    auto newEnv = std::make_shared<Environment>(interpreter->globals_);
    int i = 0;
    for (auto param : declaration_->params)
    {
        newEnv->define(param.lexeme_, arguments[i++]);
    }
    std::cout << "creating new env for n=" << std::any_cast<double>(arguments[0]) << "\n";
    interpreter->executeBlock(declaration_->body, newEnv);
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