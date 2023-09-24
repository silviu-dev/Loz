#include "Environment.hpp"
#include <iostream>
using namespace std;

void Environment::define(std::string name, std::any value)
{
    values.insert(std::make_pair(name, value));
}

std::any Environment::get(const Token &name)
{
    if (values.find(name.lexeme_) != values.end())
    {
        return values[name.lexeme_];
    }
    if (enclosing != nullptr)
        return enclosing->get(name);
    throw RunError(name, "Undefined variable '" + name.lexeme_ + "'.");
}

void Environment::assign(const Token &name, std::any value)
{

    if (values.find(name.lexeme_) != values.end())
    {
        values[name.lexeme_] = value;
    }
    else if (enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }
    else
    {
        throw RunError(name, "Undefined variable '" + name.lexeme_ + "'.");
    }
}