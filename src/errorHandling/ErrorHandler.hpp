#include "IErrorHandler.hpp"

class ErrorHandler : public IErrorHandler
{
    void signalError(int line, const std::string &message) override;
};