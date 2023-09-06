#include "IErrorHandler.hpp"

class ErrorHandler : public IErrorHandler
{
    void signalError(int line, std::string message) override;
};