#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "ErrorHandlerMock.hpp"
#include "Scanner.hpp"

using namespace testing;

class ScannerTest :public ::testing::TestWithParam<std::tuple<const std::string, Token>> {
protected:
    IScannerPtr sut_;
    std::shared_ptr<StrictMock<ErrorHandlerMock>> errorHandlerMock_ = std::make_shared<StrictMock<ErrorHandlerMock>>();
};


TEST_P(ScannerTest, ScanSucces) 
{
    auto sourceCode = std::get<0>(GetParam());
    auto expectedToken =std::get<1>(GetParam());
   sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
 	const auto& result = sut_->scanTokens();
    ASSERT_EQ(result.size(), 2);//EOF added allways
    EXPECT_EQ(result[0], expectedToken)<<result[0].toString()<<" [nu este egal cu] "<<expectedToken.toString();
}

TEST_F(ScannerTest, IgnoreCommentSucces) 
{
    std::string sourceCode("//comment");
    auto expectedToken = Token(EOFI,"\0", 0, 1);
    sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
    const auto& result = sut_->scanTokens();

    EXPECT_EQ(result.size(), 1);//EOFI
    EXPECT_EQ(result[0], expectedToken)<<result[0].toString()<<" [nu este egal cu] "<<expectedToken.toString();
}

TEST_F(ScannerTest, IgnoreWhitespaceSucces) 
{
    std::string sourceCode(" ");
    auto expectedToken = Token(EOFI,"\0", 0, 1);
    sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
    const auto& result = sut_->scanTokens();

    EXPECT_EQ(result.size(), 1);//EOFI
    EXPECT_EQ(result[0], expectedToken)<<result[0].toString()<<" [nu este egal cu] "<<expectedToken.toString();
}

TEST_F(ScannerTest, IgnoreEmptyLineSucces) 
{
    std::string sourceCode("\n");
    auto expectedToken = Token(EOFI,"\0", 0, 2);
    sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
    const auto& result = sut_->scanTokens();

    EXPECT_EQ(result.size(), 1);//EOFI
    EXPECT_EQ(result[0], expectedToken)<<result[0].toString()<<" [nu este egal cu] "<<expectedToken.toString();
}


TEST_F(ScannerTest, TryToScanTwoCharactersTokenButFaile) 
{
    std::string sourceCode("=ana");
    auto expectedToken = Token(EQUAL,"=", 0, 1);
    sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
    const auto& result = sut_->scanTokens();

    EXPECT_EQ(result.size(), 3);//+EOFI
    EXPECT_EQ(result[0], expectedToken)<<result[0].toString()<<" [nu este egal cu] "<<expectedToken.toString();
}

TEST_F(ScannerTest, DontScanNumberPointAsTrailingDecimalPointNumber) 
{
    std::string sourceCode("12.");
    auto expectedTokenNumber = Token(NUMBER,"12", std::make_shared<float>(12), 1);
    auto expectedTokenDot = Token(DOT,".", 0, 1);
    sut_ = std::make_shared<Scanner>(sourceCode, nullptr);
    const auto& result = sut_->scanTokens();

    EXPECT_EQ(result.size(), 3);//+EOFI
    EXPECT_EQ(result[0], expectedTokenNumber)<<result[0].toString()<<" [nu este egal cu] "<<expectedTokenNumber.toString();
    EXPECT_EQ(result[1], expectedTokenDot)<<result[1].toString()<<" [nu este egal cu] "<<expectedTokenDot.toString();
}

TEST_F(ScannerTest, ExpectErrorUnexpectedCharacter) 
{
    std::string sourceCode("~");
    sut_ = std::make_shared<Scanner>(sourceCode, errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(1, "Unexpected character."));
    sut_->scanTokens();
}

TEST_F(ScannerTest, ExpectErrorUnterminatedString) 
{
    std::string sourceCode("\"unterminated string");
    sut_ = std::make_shared<Scanner>(sourceCode, errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(1, "Unterminated string."));
    sut_->scanTokens();
}

INSTANTIATE_TEST_CASE_P(
        ScannerTestParameters,
        ScannerTest,
        ::testing::Values(
            std::make_tuple("(", Token(LEFT_PAREN,"(", 0, 1)),
            std::make_tuple(")", Token(RIGHT_PAREN,")", 0, 1)),
            std::make_tuple("{", Token(LEFT_BRACE,"{", 0, 1)),
            std::make_tuple("}", Token(RIGHT_BRACE,"}", 0, 1)),
            std::make_tuple(",", Token(COMMA,",", 0,1)),
            std::make_tuple(".", Token(DOT,".", 0, 1)),
            std::make_tuple("-", Token(MINUS,"-", 0,1)),
            std::make_tuple("+", Token(PLUS,"+", 0,1)),
            std::make_tuple(";", Token(SEMICOLON,";", 0,1)),
            std::make_tuple("/", Token(SLASH,"/", 0,1)),
            std::make_tuple("*", Token(STAR,"*", 0,1)),
            std::make_tuple("!", Token(BANG,"!", 0,1)),
            std::make_tuple("!=", Token(BANG_EQUAL,"!=", 0,1)),
            std::make_tuple("=", Token(EQUAL,"=", 0,1)),
            std::make_tuple("==", Token(EQUAL_EQUAL,"==", 0,1)),
            std::make_tuple(">", Token(GREATER,">", 0,1)),
            std::make_tuple(">=", Token(GREATER_EQUAL,">=", 0,1)),
            std::make_tuple("<", Token(LESS,"<", 0,1)),
            std::make_tuple("<=", Token(LESS_EQUAL,"<=", 0,1)),
            std::make_tuple("myVariable", Token(IDENTIFIER,"myVariable", 0,1)),
            std::make_tuple("\"multiline\nstring\"", Token(STRING,"\"multiline\nstring\"", std::make_shared<std::string>("multiline\nstring"),1)),
            std::make_tuple("1999.0", Token(NUMBER,"1999.0", std::make_shared<float>(1999),1)),
            std::make_tuple("and", Token(AND,"and", 0,1)),
            std::make_tuple("class", Token(CLASS,"class", 0,1)),
            std::make_tuple("else", Token(ELSE,"else", 0,1)),
            std::make_tuple("false", Token(FALSE,"false", 0,1)),
            std::make_tuple("fun", Token(FUN,"fun", 0,1)),
            std::make_tuple("for", Token(FOR,"for", 0, 1)),
            std::make_tuple("if", Token(IF,"if", 0,1)),
            std::make_tuple("nil", Token(NIL,"nil", 0,1)),
            std::make_tuple("or", Token(OR,"or", 0,1)),
            std::make_tuple("print", Token(PRINT, "print", 0,1)),
            std::make_tuple("return", Token(RETURN,"return", 0,1)),
            std::make_tuple("super", Token(SUPER,"super", 0,1)),
            std::make_tuple("this", Token(THIS,"this", 0,1)),
            std::make_tuple("true", Token(TRUE,"true", 0,1)),
            std::make_tuple("var", Token(VAR,"var", 0,1)),
            std::make_tuple("while", Token(WHILE,"while", 0,1))
        ));
