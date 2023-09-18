#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "ErrorHandlerMock.hpp"
#include "Parser.hpp"

using namespace testing;

class ParserTest :public ::testing::TestWithParam<std::tuple<const std::string, Token>> {
protected:
    std::shared_ptr<Parser> sut_;
    std::shared_ptr<StrictMock<ErrorHandlerMock>> errorHandlerMock_ = std::make_shared<StrictMock<ErrorHandlerMock>>();
};

TEST_F(ParserTest, IgnoreCommentSucces) 
{std::vector<Token> tokens{};
    sut_ = std::make_shared<Parser>(tokens, nullptr);
    sut_->parse();
}