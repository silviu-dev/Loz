#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ErrorHandler.hpp"

using namespace testing;

class ErrorHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override 
  {
  	sut_ = std::make_shared<ErrorHandler>();
  }
  IErrorHandlerPtr sut_;
};

TEST_F(ErrorHandlerTest, errorWithLineAndMessageSucces) 
{
	std::string errorMessage = "expected a ;";

    testing::internal::CaptureStdout();
    sut_->error(99, errorMessage);
    std::string output = testing::internal::GetCapturedStdout();
  	EXPECT_EQ(output, "[line 99] Error: expected a ;\n");
}

TEST_F(ErrorHandlerTest, errorWithTokenAndMessageSucces) 
{
    std::string errorMessage = "Operands must be either numbers or strings";

    testing::internal::CaptureStdout();
    sut_->error(Token(PLUS,"+", 0,1), errorMessage);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[line 1] Error at '+': Operands must be either numbers or strings\n");
}

TEST_F(ErrorHandlerTest, errorWithTokenEOFISucces) 
{
    std::string errorMessage = "Empty file";

    testing::internal::CaptureStdout();
    sut_->error(Token(EOFI,"\0", 0, 1), errorMessage);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[line 1] Error at end: Empty file\n");
}