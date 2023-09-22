#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <typeinfo>
#include <any>

#include "ErrorHandlerMock.hpp"
#include "Interpreter.hpp"

using namespace testing;

class InterpreterTest :public ::testing::TestWithParam<std::tuple<std::shared_ptr<Expr>, std::any>> {
protected:
    std::shared_ptr<Interpreter> sut_;
    std::shared_ptr<StrictMock<ErrorHandlerMock>> errorHandlerMock_ = std::make_shared<StrictMock<ErrorHandlerMock>>();
};

TEST_F(InterpreterTest, StringMinusStringFail) 
{   auto expr = std::make_shared<Binary>(std::make_shared<Literal>(std::string("factor1")), Token(MINUS,"-", 0,1) ,std::make_shared<Literal>(std::string("factor2")));
    sut_ = std::make_shared<Interpreter>(errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(Token(MINUS,"-", 0,1), "Operands must be numbers."));
    sut_->interpret(expr);
    auto result = sut_->getResult();
    if (result.type() == typeid(nullptr))
    {
        auto convertedResult = std::any_cast<nullptr_t>(result);
        EXPECT_EQ(convertedResult, nullptr);
    }
    else
    {
        FAIL()<<"result isn't equal with nullptr";
    }
}

TEST_F(InterpreterTest, MinusStringFail) 
{   auto expr = std::make_shared<Unary>(Token(MINUS,"-", 0,1),std::make_shared<Literal>(std::string("string")));
    sut_ = std::make_shared<Interpreter>(errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(Token(MINUS,"-", 0,1), "Operand must be a number."));
    sut_->interpret(expr);
    auto result = sut_->getResult();
    if (result.type() == typeid(nullptr))
    {
        auto convertedResult = std::any_cast<nullptr_t>(result);
        EXPECT_EQ(convertedResult, nullptr);
    }
    else
    {
        FAIL()<<"result isn't equal with nullptr";
    }
}

TEST_F(InterpreterTest, AddingBooleanValuesFail) 
{   auto expr = std::make_shared<Binary>(std::make_shared<Literal>(true), Token(PLUS,"+", 0,1) ,std::make_shared<Literal>(false));
    sut_ = std::make_shared<Interpreter>(errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(Token(PLUS,"+", 0,1), "Operands must be either numbers or strings"));
    sut_->interpret(expr);
    auto result = sut_->getResult();
    if (result.type() == typeid(nullptr))
    {
        auto convertedResult = std::any_cast<nullptr_t>(result);
        EXPECT_EQ(convertedResult, nullptr);
    }
    else
    {
        FAIL()<<"result isn't equal with nullptr";
    }
}

TEST_P(InterpreterTest, InterpretExpressionSucces) 
{   auto expr = std::get<0>(GetParam());
    auto expectedValue = std::get<1>(GetParam());
    sut_ = std::make_shared<Interpreter>(errorHandlerMock_);
    sut_->interpret(expr);
    auto result = sut_->getResult();
    if (result.type() == typeid(double))
    {
        auto convertedResult = std::any_cast<double>(result);
        auto convertedExpectedResult = std::any_cast<double>(expectedValue);
        EXPECT_EQ(convertedResult, convertedExpectedResult);
    }
    else if (result.type() == typeid(std::string))
    {
        auto convertedResult = std::any_cast<std::string>(result);
        auto convertedExpectedResult = std::any_cast<std::string>(expectedValue);
        EXPECT_EQ(convertedResult, convertedExpectedResult);
    }
    else if (result.type() == typeid(bool))
    {
        auto convertedResult = std::any_cast<bool>(result);
        auto convertedExpectedResult = std::any_cast<bool>(expectedValue);
        EXPECT_EQ(convertedResult, convertedExpectedResult);
    }

}

INSTANTIATE_TEST_CASE_P(
        InterpreterTestParameters,
        InterpreterTest,
        ::testing::Values(
            std::make_tuple(std::make_shared<Literal>(false), false), // FALSE literal expression: false --> false
            std::make_tuple(std::make_shared<Literal>(true), true), // TRUE literal expression: true --> true
            std::make_tuple(std::make_shared<Literal>(nullptr), nullptr), // NIL literal expression: nill --> nullptr
            std::make_tuple(std::make_shared<Literal>(1999.0), 1999.0), // NUMBER literal expression: 1999 --> 1999
            std::make_tuple(std::make_shared<Literal>(std::string("string")), std::string("string")), // STRING literal expression: "string" --> "string"
            std::make_tuple(std::make_shared<Unary>(Token(BANG,"!", 0,1),std::make_shared<Literal>(false)), true), // BANG unary expression: !false --> true
            std::make_tuple(std::make_shared<Unary>(Token(BANG,"!", 0,1),std::make_shared<Literal>(std::string("string"))), false), // BANG unary expression: !"string" --> false
            std::make_tuple(std::make_shared<Unary>(Token(BANG,"!", 0,1),std::make_shared<Literal>(nullptr)), true), // BANG unary expression: !nil --> true
            std::make_tuple(std::make_shared<Unary>(Token(MINUS,"-", 0,1),std::make_shared<Literal>(2.0)), -2.0), // MINUS unary expression: -2 --> -2
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(SLASH,"/", 0,1) ,std::make_shared<Literal>(2.0)), 1.0), // SLASH binary factor expression: 2/2 --> 1
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(STAR,"*", 0,1) ,std::make_shared<Literal>(2.0)), 4.0), // STAR binary factor expression: 2*2 --> 4
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(MINUS,"-", 0,1) ,std::make_shared<Literal>(2.0)), 0.0), // MINUS binary term expression: 2-2 --> 0
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(PLUS,"+", 0,1) ,std::make_shared<Literal>(2.0)), 4.0), // PLUS binary term expression: 2+2 --> 4
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(std::string("Ana are ")), Token(PLUS,"+", 0,1) ,std::make_shared<Literal>(std::string("prune"))), std::string("Ana are prune")),// PLUS binary term expression: "Ana are " + "prune" --> "Ana are prune"
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(GREATER,">", 0,1) ,std::make_shared<Literal>(2.0)), false), // GRATER binary comparation expression: 2>2 --> false
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(GREATER_EQUAL,">=", 0,1) ,std::make_shared<Literal>(2.0)), true), // GREATER_EQUAL binary comparation expression: 2>=2 --> true
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(LESS,"<", 0,1) ,std::make_shared<Literal>(2.0)), false), // LESS binary comparation expression: 2<2 --> false
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(LESS_EQUAL,"<=", 0,1) ,std::make_shared<Literal>(2.0)), true), // LESS_EQUAL binary comparation expression: 2<=2 --> true
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(BANG_EQUAL,"!=", 0,1) ,std::make_shared<Literal>(2.0)), false), // BANG_EQUAL binary comparation expression: 2!=2 --> false
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(EQUAL_EQUAL,"==", 0,1) ,std::make_shared<Literal>(2.0)), true), // EQUAL_EQUAL binary comparation expression: 2==2 --> true
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(EQUAL_EQUAL,"==", 0,1) ,std::make_shared<Literal>(std::string("2"))), false), // EQUAL_EQUAL binary comparation expression: 2=="2" --> false
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(std::string("2")), Token(EQUAL_EQUAL,"==", 0,1) ,std::make_shared<Literal>(std::string("2"))), true), // EQUAL_EQUAL binary comparation expression: "2"=="2" --> true
            std::make_tuple(std::make_shared<Binary>(std::make_shared<Literal>(false), Token(EQUAL_EQUAL,"==", 0,1) ,std::make_shared<Literal>(false)), true), // EQUAL_EQUAL binary comparation expression: false==false --> true
            std::make_tuple(std::make_shared<Grouping>(std::make_shared<Literal>(2.0)), 2.0) // bracketed NUMBER literal expression: (2) --> 2
        ));