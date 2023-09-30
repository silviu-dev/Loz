#pragma once
#include <any>
#include <iostream>
#include <map>
#include <string>
using namespace std;
#include "Token.hpp"

struct RunError : public std::exception
{
    RunError(const Token &oper, std::string message) : oper(oper), message(message)
    {
    }
    Token oper;
    std::string message;
};

class Environment
{
  public:
    Environment() : enclosing(nullptr)
    {
    }
    Environment(std::shared_ptr<Environment> env) : enclosing(env)
    {
    }
    virtual void define(std::string name, std::any value);
    virtual std::any get(const Token &name);
    virtual void assign(const Token &name, std::any value);

    std::shared_ptr<Environment> enclosing;

  private:
    std::map<std::string, std::any> values;
};