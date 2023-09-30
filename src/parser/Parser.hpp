#pragma once
#include <memory>
#include <vector>

#include "IErrorHandler.hpp"
#include "IScanner.hpp"
#include "Stmt.hpp"

struct Parser
{
    Parser(std::vector<Token> &tokens, const std::shared_ptr<IErrorHandler> &errorHandler)
        : tokens_(tokens), current_(0), errorHandler_(errorHandler)
    {
    }

    std::vector<std::shared_ptr<Stmt>> parse();

  private:
    class ParseError : public std::exception
    {
    };

    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> function(std::string kind);
    std::shared_ptr<Stmt> varDeclaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Stmt> returnStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::shared_ptr<Stmt> forStatement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Stmt> expressionStatement();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> orOperation();
    std::shared_ptr<Expr> andOperation();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();

    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, std::string message);
    ParseError error(Token token, std::string message);
    void synchronize();
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);

    std::vector<Token> tokens_;
    int current_;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
};