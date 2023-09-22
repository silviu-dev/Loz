#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ErrorHandlerMock.hpp"
#include "Token.hpp"

using namespace testing;

class TokenTest :public ::testing::Test {
protected:
    std::shared_ptr<Token> sut_;
};

TEST_F(TokenTest, tokenToStringSucces) 
{
    sut_ = std::make_shared<Token>(NUMBER,"2", 2.0,1);
    EXPECT_EQ(sut_->toString(), std::string("NUMBER 2 1"));
}

TEST_F(TokenTest, tokenAssigmentSucces) 
{
    Token t1(NUMBER,"2", 2.0,1);
    Token t2(STRING,"\"string\"", std::string("string"),1);
    t2=t1;
    EXPECT_EQ(t2, t1);
}

TEST_F(TokenTest, compareTokensWithDifferentTokenTypeFail) 
{
    Token t1(NUMBER,"2", 2.0,1);
    Token t2(STRING,"\"string\"", std::string("string"),1);
    EXPECT_EQ(t2==t1, false);
}

TEST_F(TokenTest, compareStringTokensWithDifferentLiteralFail) 
{
    Token t1(STRING,"\"string1\"", std::string("string1"),1);
    Token t2(STRING,"\"string2\"", std::string("string2"),1);
    EXPECT_EQ(t2==t1, false);
}

TEST_F(TokenTest, compareNumberTokensWithDifferentLiteralFail) 
{
    Token t1(NUMBER,"2", 2.0,1);
    Token t2(NUMBER,"3", 3.0,1);
    EXPECT_EQ(t2==t1, false);
}

TEST_F(TokenTest, compareBoolTokensWithDifferentLiteralFail) 
{
    Token t1(IDENTIFIER,"true", true ,1);
    Token t2(IDENTIFIER,"false", false ,1);
    EXPECT_EQ(t2==t1, false);
}