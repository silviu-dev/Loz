#pragma once
#include <any>
#include <array>
#include <memory>
#include <string>
#include <typeinfo>
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
    Token(TokenType type, std::string lexeme, std::any literal, int line);

    std::string toString() const;
    std::string convertTokeTypeToString(TokenType t) const;
    bool operator==(const Token &token) const;
    void operator=(const Token &token);
    Token(const Token &token);
    TokenType type_;

  public:
    std::string lexeme_ = "";
    std::any literal_;
    int line_ = 0;
};