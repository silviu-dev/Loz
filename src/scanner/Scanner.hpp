#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "IErrorHandler.hpp"
#include "IScanner.hpp"

class Scanner : public IScanner
{
  public:
    Scanner(const std::string &source, const std::shared_ptr<IErrorHandler> &errorHandler)
        : source_(source), errorHandler_(errorHandler)
    {
    }
    std::vector<Token> scanTokens() override;

  private:
    void scanToken();
    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal, int line = 0);
    bool match(char expected);
    char peek();
    void string();
    bool isDigit(char c);
    void number();
    char peekNext();
    void identifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    std::string source_ = "";
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
    std::vector<Token> tokens_ = {};
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;
};
