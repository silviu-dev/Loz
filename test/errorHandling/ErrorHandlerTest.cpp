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

TEST_F(ErrorHandlerTest, errorSucces) 
{
	std::string errorMessage = "expected a ;";

    testing::internal::CaptureStdout();
    sut_->error(99, errorMessage);
    std::string output = testing::internal::GetCapturedStdout();
  	EXPECT_EQ(output, "[line 99] Error: expected a ;\n");
}