#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Scanner.hpp"

using namespace testing;

class ScannerTest : public ::testing::Test {
 protected:
  void SetUp() override 
  {
  	sut_ = std::make_shared<Scanner>();
  }
  IScannerPtr sut_;
};

TEST_F(ScannerTest, ScanSucces) 
{
	std::string input = "if(2==1+1)";
  	const auto& result = sut_->scanTokens(input);
  	EXPECT_THAT(result, IsEmpty());
}