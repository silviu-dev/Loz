#pragma once
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
    Token(TokenType type, std::string lexeme, void *literal, int line)
        : type_(type), lexeme_(lexeme), literal_(literal), line_(line)
    {
    }

    ~Token()
    {
        if (literal_ != NULL)
        {
            if (type_ == STRING)
            {
                auto stringPtr = static_cast<std::string *>(literal_);
                literal_ = NULL;
                delete stringPtr;
            }
            else if (type_ == NUMBER)
            {
                auto floatPtr = static_cast<float *>(literal_);
                literal_ = NULL;
                delete (floatPtr);
            }
        }
    }

    std::string toString() const
    {
        return convertTokeTypeToString(type_) + " " + lexeme_ + " " + std::to_string(line_); //+ " " + literal_;
    }

    std::string convertTokeTypeToString(TokenType t) const
    {
        return tokenTypeString[t];
    }
    Token(const Token &token)
    {
        type_ = token.type_;
        lexeme_ = token.lexeme_;
        line_ = token.line_;
        if (token.type_ == STRING)
        {
            auto stringPtr = static_cast<std::string *>(token.literal_);
            literal_ = new std::string(*stringPtr);
        }
        else if (token.type_ == NUMBER)
        {
            auto floatPtr = static_cast<float *>(token.literal_);
            literal_ = new float(*floatPtr);
        }
    }
    Token operator=(const Token &token)
    {
        if (this != &token)
        {
            type_ = token.type_;
            lexeme_ = token.lexeme_;
            line_ = token.line_;
            if (token.type_ == STRING)
            {
                auto stringPtr = static_cast<std::string *>(token.literal_);
                literal_ = new std::string(*stringPtr);
            }
            else if (token.type_ == NUMBER)
            {
                auto floatPtr = static_cast<float *>(token.literal_);
                literal_ = new float(*floatPtr);
            }
        }
        return *this;
    }
    bool operator==(const Token &token) const
    {
        if (token.type_ == STRING)
        {
            auto l = static_cast<std::string *>(token.literal_);
            auto r = static_cast<std::string *>(literal_);
            if (l == NULL || r == NULL || *l != *r)
            {
                return false;
            }
        }
        if (token.type_ == NUMBER)
        {
            auto l = static_cast<float *>(token.literal_);
            auto r = static_cast<float *>(literal_);
            if (l == NULL || r == NULL || *l != *r)
                return false;
        }
        return token.type_ == type_ && token.lexeme_ == lexeme_ && token.line_ == line_;
    }

    TokenType type_;
    std::string lexeme_ = "";

  public:
    void *literal_ = NULL;
    int line_ = 0;
};

struct IScanner
{
    virtual std::vector<Token> scanTokens() = 0;
};

using IScannerPtr = std::shared_ptr<IScanner>;