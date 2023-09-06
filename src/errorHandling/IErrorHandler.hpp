#pragma once

#include <memory>
#include <string>

struct IErrorHandler
{
    virtual void signalError(int line, std::string message) = 0;
};

using IErrorHandlerPtr = std::shared_ptr<IErrorHandler>;