#include <iostream>

#include "ErrorHandler.hpp"

void ErrorHandler::error(int line, const std::string &message)
{
    report(line, "", message);
}

void ErrorHandler::error(Token token, const std::string &message)
{
    if (token.type_ == EOFI)
    {
        report(token.line_, " at end", message);
    }
    else
    {
        report(token.line_, " at '" + token.lexeme_ + "'", message);
    }
}

void ErrorHandler::report(int line, std::string where, std::string message)
{
    std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
}