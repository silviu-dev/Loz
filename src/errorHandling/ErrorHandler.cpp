#include <iostream>

#include "ErrorHandler.hpp"

void ErrorHandler::signalError(int line, const std::string &message)
{
    std::cout << "[line " << line << "]Error:" << message << "\n";
}