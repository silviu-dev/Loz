#pragma once
#include <list>
#include <memory>

struct Token
{
};
struct IScanner
{
    virtual std::list<Token> scanTokens(const std::string &) = 0;
};

using IScannerPtr = std::shared_ptr<IScanner>;