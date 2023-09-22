#include "Token.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type_(type), lexeme_(lexeme), literal_(literal), line_(line)
{
}

std::string Token::toString() const
{
    return convertTokeTypeToString(type_) + " " + lexeme_ + " " + std::to_string(line_); //+ " " + literal_;
}

std::string Token::convertTokeTypeToString(TokenType t) const
{
    return tokenTypeString[t];
}

bool Token::operator==(const Token &token) const
{
    if (token.type_ != type_)
        return false;
    if (token.literal_.type() == typeid(std::string))
    {
        auto l = std::any_cast<std::string>(token.literal_);
        auto r = std::any_cast<std::string>(literal_);
        if (l != r)
            return false;
    }
    if (token.literal_.type() == typeid(double))
    {
        auto l = std::any_cast<double>(token.literal_);
        auto r = std::any_cast<double>(literal_);
        if (l != r)
            return false;
    }
    if (token.literal_.type() == typeid(bool))
    {
        auto l = std::any_cast<bool>(token.literal_);
        auto r = std::any_cast<bool>(literal_);
        if (l != r)
            return false;
    }
    return token.type_ == type_ && token.lexeme_ == lexeme_ && token.line_ == line_;
}

Token::Token(const Token &token)
{
    type_ = token.type_;
    lexeme_ = token.lexeme_;
    line_ = token.line_;
    literal_ = token.literal_;
}
void Token::operator=(const Token &token)
{
    if (this != &token)
    {
        type_ = token.type_;
        lexeme_ = token.lexeme_;
        line_ = token.line_;
        literal_ = token.literal_;
    }
}