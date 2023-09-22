#pragma once
#include <memory>
#include <vector>

#include "Token.hpp"

struct IScanner
{
    virtual std::vector<Token> scanTokens() = 0;
};

using IScannerPtr = std::shared_ptr<IScanner>;