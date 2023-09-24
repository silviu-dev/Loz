#include "Parser.hpp"
#include <iostream>
#include <memory>
using namespace std;

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
    std::vector<std::shared_ptr<Stmt>> statements{};
    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration()
{
    try
    {
        if (match({VAR}))
            return varDeclaration();
        return statement();
    }
    catch (ParseError error)
    {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::varDeclaration()
{
    consume(IDENTIFIER, "Expect variable name.");
    auto name = previous();
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({EQUAL}))
    {
        initializer = expression();
    }
    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement()
{
    if (match({PRINT}))
        return printStatement();
    if (match({WHILE}))
        return whileStatement();
    if (match({LEFT_BRACE}))
        return std::make_shared<Block>(block());
    if (match({FOR}))
        return forStatement();
    if (match({IF}))
        return ifStatement();
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::forStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    std::shared_ptr<Stmt> initializer;
    if (match({SEMICOLON}))
    {
        initializer = nullptr;
    }
    else if (match({VAR}))
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }
    std::shared_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON))
    {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");
    std::shared_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    std::shared_ptr<Stmt> body = statement();

    if (increment != nullptr)
    {
        auto vec = std::vector<std::shared_ptr<Stmt>>{body, std::make_shared<Expression>(increment)};
        body = std::make_shared<Block>(vec);
    }
    if (condition == nullptr)
    {
        condition = std::make_shared<Literal>(true);
    }
    body = std::make_shared<While>(condition, body);
    if (initializer != nullptr)
    {
        body = std::make_shared<Block>(std::vector<std::shared_ptr<Stmt>>{initializer, body});
    }
    return body;
}

std::shared_ptr<Stmt> Parser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement();
    return std::make_shared<While>(condition, body);
}

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
    std::vector<std::shared_ptr<Stmt>> statements{};
    while (!check(RIGHT_BRACE) && !isAtEnd())
    {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::printStatement()
{
    auto value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::ifStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");
    auto thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match({ELSE}))
    {
        elseBranch = statement();
    }
    return std::make_shared<If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::expressionStatement()
{
    auto expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
    auto expr = orOperation();
    if (match({EQUAL}))
    {
        auto equals = previous();
        auto value = assignment();
        auto exprConverted = std::dynamic_pointer_cast<Variable>(expr);
        if (exprConverted != nullptr)
        {
            Token name = exprConverted->name;
            return std::make_shared<Assign>(name, value);
        }
        throw error(equals, "Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::orOperation()
{
    auto expr = andOperation();
    while (match({OR}))
    {
        auto oper = previous();
        auto right = andOperation();
        expr = std::make_shared<Logical>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::andOperation()
{
    auto expr = equality();
    while (match({AND}))
    {
        auto oper = previous();
        auto right = equality();
        expr = std::make_shared<Logical>(expr, oper, right);
    }
    return expr;
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
    if (match({NIL}))
        return std::make_shared<Literal>(nullptr);
    if (match({IDENTIFIER}))
    {
        return std::make_shared<Variable>(previous());
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
    throw error(previous(), "Expect expression.");
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
    return peek().type_ == EOFI;
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
    throw error(previous(), message);
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