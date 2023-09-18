#pragma once

#include <memory>
#include <string>

#include "IScanner.hpp"

struct IErrorHandler
{
    virtual void error(Token token, const std::string &message) = 0;
    virtual void error(int line, const std::string &message) = 0;
};

using IErrorHandlerPtr = std::shared_ptr<IErrorHandler>;