#include "Parser.hpp"

std::shared_ptr<Expr> Parser::parse()
{
    try
    {
        return expression();
    }
    catch (ParseError error)
    {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
    auto expr = comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        auto oper = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    auto expr = term();
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}))
    {
        auto oper = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term()
{
    auto expr = factor();
    while (match({MINUS, PLUS}))
    {
        auto oper = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    auto expr = unary();
    while (match({SLASH, STAR}))
    {
        auto oper = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if (match({BANG, MINUS}))
    {
        auto oper = previous();
        auto right = unary();
        return std::make_shared<Unary>(oper, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
    if (match({FALSE}))
        return std::make_shared<Literal>(false);
    if (match({TRUE}))
        return std::make_shared<Literal>(true);
    if (match({IDENTIFIER}))
    {
        return std::make_shared<Literal>(nullptr);
    }
    if (match({NUMBER}))
    {
        return std::make_shared<Literal>(previous().literal_);
    }
    if (match({STRING}))
        return std::make_shared<Literal>(previous().literal_);
    if (match({LEFT_PAREN}))
    {
        auto expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }
    throw error(peek(), "Expect expression.");
}

bool Parser::match(std::vector<TokenType> types)
{
    for (TokenType type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek().type_ == type;
}

void Parser::advance()
{
    if (!isAtEnd())
        current_++;
}

bool Parser::isAtEnd()
{
    return peek().type_ == EOF;
}

Token Parser::peek()
{
    return tokens_[current_];
}

Token Parser::previous()
{
    return tokens_[current_ - 1];
}

void Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();
    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, std::string message)
{
    errorHandler_->error(token, message);
    return ParseError();
}

void Parser::synchronize()
{
    advance();
    while (!isAtEnd())
    {
        if (previous().type_ == SEMICOLON)
            return;
        switch (peek().type_)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }
        advance();
    }
}