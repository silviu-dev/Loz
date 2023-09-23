#pragma once
#include <any>
#include <map>
#include <string>

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
    void define(std::string name, std::any value);
    std::any get(const Token &name);
    void assign(const Token &name, std::any value);

  private:
    std::map<std::string, std::any> values;
};