#pragma once
#include <any>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

const std::array<std::string, 40> tokenTypeString = {
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH",
    "STAR", "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
    // Literals.
    "IDENTIFIER", "STRING", "NUMBER",
    // Keywords.
    "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR",
    "WHILE", "EOFI"};

enum TokenType
{
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,
    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    EOFI
};

class Token
{
  public:
    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type_(type), lexeme_(lexeme), literal_(literal), line_(line)
    {
    }

    std::string toString() const
    {
        return convertTokeTypeToString(type_) + " " + lexeme_ + " " + std::to_string(line_); //+ " " + literal_;
    }

    std::string convertTokeTypeToString(TokenType t) const
    {
        return tokenTypeString[t];
    }

    bool operator==(const Token &token) const
    {
        if (token.type_ != type_)
            return false;
        if (token.type_ == STRING)
        {
            auto l = std::any_cast<std::string>(token.literal_);
            auto r = std::any_cast<std::string>(literal_);
            if (l != r)
                return false;
        }
        if (token.type_ == NUMBER)
        {
            auto l = std::any_cast<double>(token.literal_);
            auto r = std::any_cast<double>(literal_);
            if (l != r)
                return false;
        }
        return token.type_ == type_ && token.lexeme_ == lexeme_ && token.line_ == line_;
    }

    Token(const Token &token)
    {
        type_ = token.type_;
        lexeme_ = token.lexeme_;
        line_ = token.line_;
        literal_ = token.literal_;
    }
    Token operator=(const Token &token)
    {
        if (this != &token)
        {
            type_ = token.type_;
            lexeme_ = token.lexeme_;
            line_ = token.line_;
            literal_ = token.literal_;
        }
        return *this;
    }
    TokenType type_;

  public:
    std::string lexeme_ = "";
    std::any literal_;
    int line_ = 0;
};

struct IScanner
{
    virtual std::vector<Token> scanTokens() = 0;
};

using IScannerPtr = std::shared_ptr<IScanner>;