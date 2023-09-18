#include "Scanner.hpp"

std::map<std::string, TokenType> keywords = {{"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
                                             {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
                                             {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
                                             {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        // We are at the beginning of the next lexeme.
        start_ = current_;
        scanToken();
    }
    tokens_.push_back(Token(EOFI, "", 0, line_));
    return tokens_;
}

bool Scanner::isAtEnd()
{
    return current_ >= source_.length();
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case ';':
        addToken(SEMICOLON);
        break;
    case '*':
        addToken(STAR);
        break;
    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (match('/'))
        {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd())
                advance();
        }
        else
        {
            addToken(SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line_++;
        break;
    case '"':
        string();
        break;
    default:
        if (isDigit(c))
        {
            number();
        }
        else if (isAlpha(c))
        {
            identifier();
        }
        else
        {
            errorHandler_->error(line_, "Unexpected character.");
        }
        break;
    }
}

char Scanner::advance()
{
    current_++;
    return source_[current_ - 1];
}

void Scanner::addToken(TokenType type)
{
    addToken(type, NULL);
}

void Scanner::addToken(TokenType type, std::any literal, int line)
{
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back(Token(type, text, literal, line ? line : line_));
}

bool Scanner::match(char expected)
{
    if (isAtEnd())
        return false;
    if (source_[current_] != expected)
        return false;
    current_++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source_[current_];
}

void Scanner::string()
{
    int startingLine = line_;
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line_++;
        advance();
    }
    if (isAtEnd())
    {
        errorHandler_->error(line_, "Unterminated string.");
        return;
    }
    // The closing ".
    advance();
    // Trim the surrounding quotes.
    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    addToken(STRING, std::any(value), startingLine);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

void Scanner::number()
{
    while (isDigit(peek()))
        advance();
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext()))
    {
        // Consume the "."
        advance();
        while (isDigit(peek()))
            advance();
    }
    addToken(NUMBER, std::any(stod(source_.substr(start_, current_ - start_))));
}

char Scanner::peekNext()
{
    if (current_ + 1 >= source_.length())
        return '\0';
    return source_[current_ + 1];
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    std::string text = source_.substr(start_, current_ - start_);
    auto type = keywords.find(text);
    if (type == keywords.end())
        addToken(IDENTIFIER);
    else
        addToken(type->second);
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}
