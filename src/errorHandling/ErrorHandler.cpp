#include <iostream>

#include "ErrorHandler.hpp"

void ErrorHandler::signalError(int line, std::string message)
{
    std::cout << "[line " << line << "]Error:" << message;
}