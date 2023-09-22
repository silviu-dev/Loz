#pragma once
#include <memory>

#include "Expr.hpp"
#include "IErrorHandler.hpp"
#include "IScanner.hpp"

struct Parser
{
    Parser(std::vector<Token> &tokens, const std::shared_ptr<IErrorHandler> &errorHandler)
        : tokens_(tokens), current_(0), errorHandler_(errorHandler)
    {
    }

    std::shared_ptr<Expr> parse();

  private:
    class ParseError : public std::exception
    {
    };

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    void advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    void consume(TokenType type, std::string message);
    ParseError error(Token token, std::string message);
    void synchronize();

    std::vector<Token> tokens_;
    int current_;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
};