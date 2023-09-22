#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <typeinfo>

#include "ErrorHandlerMock.hpp"
#include "Parser.hpp"

using namespace testing;

class ParserTest :public ::testing::TestWithParam<std::tuple<std::vector<Token>, std::shared_ptr<Expr>>> {
protected:
    std::shared_ptr<Parser> sut_;
    std::shared_ptr<StrictMock<ErrorHandlerMock>> errorHandlerMock_ = std::make_shared<StrictMock<ErrorHandlerMock>>();
};

TEST_F(ParserTest, ParseIncompleteExpressionFail) 
{
    std::vector<Token> tokens={Token(NUMBER,"2", 2.0,1),Token(PLUS,"+", 0, 1), Token(EOFI,"\0", 0, 1)};
    sut_ = std::make_shared<Parser>(tokens, errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(Token(EOFI,"\0", 0, 1), "Expect expression."));
    EXPECT_EQ(sut_->parse(), nullptr);
}

TEST_F(ParserTest, ParseUnmatchedGroupingFail) 
{
    std::vector<Token> tokens={Token(LEFT_PAREN,"(", 0, 1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)};
    sut_ = std::make_shared<Parser>(tokens, errorHandlerMock_);
    EXPECT_CALL(*errorHandlerMock_, error(Token(EOFI,"\0", 0, 1), "Expect ')' after expression."));
    EXPECT_EQ(sut_->parse(), nullptr);
}

TEST_P(ParserTest, ParseSucces) 
{
    auto tokens=std::get<0>(GetParam());
    auto expectedExpr = std::get<1>(GetParam());
    sut_ = std::make_shared<Parser>(tokens, errorHandlerMock_);
    EXPECT_EQ(sut_->parse()->isEqual(expectedExpr), true);
}


INSTANTIATE_TEST_CASE_P(
        ParserTestParameters,
        ParserTest,
        ::testing::Values(
            std::make_tuple(std::vector<Token>{Token(FALSE,"false", 0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Literal>(false)), // FALSE literal expression
            std::make_tuple(std::vector<Token>{Token(TRUE,"true", 0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Literal>(true)), // TRUE literal expression 
            std::make_tuple(std::vector<Token>{Token(IDENTIFIER,"nil", 0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Literal>(nullptr)), // NIL literal expression
            std::make_tuple(std::vector<Token>{Token(NUMBER,"1999", 1999.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Literal>(1999.0)), // NUMBER literal expression
            std::make_tuple(std::vector<Token>{Token(STRING,"\"string\"", std::string("string"),1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Literal>(std::string("string"))), // STRING literal expression
            std::make_tuple(std::vector<Token>{Token(BANG,"!", 0,1), Token(FALSE,"false", 0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Unary>(Token(BANG,"!", 0,1),std::make_shared<Literal>(false))), // BANG unary expression: !false
            std::make_tuple(std::vector<Token>{Token(MINUS,"-", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Unary>(Token(MINUS,"-", 0,1),std::make_shared<Literal>(2.0))), // MINUS unary expression: -2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(SLASH,"/", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(SLASH,"/", 0,1) ,std::make_shared<Literal>(2.0))), // SLASH binary factor expression: 2/2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(STAR,"*", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(STAR,"*", 0,1) ,std::make_shared<Literal>(2.0))), // STAR binary factor expression: 2*2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(MINUS,"-", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(MINUS,"-", 0,1) ,std::make_shared<Literal>(2.0))), // MINUS binary term expression: 2-2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(PLUS,"+", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(PLUS,"+", 0,1) ,std::make_shared<Literal>(2.0))), // PLUS binary term expression: 2+2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(GREATER,">", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(GREATER,">", 0,1) ,std::make_shared<Literal>(2.0))), // GRATER binary comparation expression: 2>2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(GREATER_EQUAL,">=", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(GREATER_EQUAL,">=", 0,1) ,std::make_shared<Literal>(2.0))), // GREATER_EQUAL binary comparation expression: 2>=2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(LESS,"<", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(LESS,"<", 0,1) ,std::make_shared<Literal>(2.0))), // LESS binary comparation expression: 2<2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(LESS_EQUAL,"<=", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(LESS_EQUAL,"<=", 0,1) ,std::make_shared<Literal>(2.0))), // LESS_EQUAL binary comparation expression: 2<=2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(BANG_EQUAL,"!=", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(BANG_EQUAL,"!=", 0,1) ,std::make_shared<Literal>(2.0))), // BANG_EQUAL binary comparation expression: 2!=2
            std::make_tuple(std::vector<Token>{Token(NUMBER,"2", 2.0,1), Token(EQUAL_EQUAL,"==", 0,1), Token(NUMBER,"2", 2.0,1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Binary>(std::make_shared<Literal>(2.0), Token(EQUAL_EQUAL,"==", 0,1) ,std::make_shared<Literal>(2.0))), // EQUAL_EQUAL binary comparation expression: 2==2
            std::make_tuple(std::vector<Token>{Token(LEFT_PAREN,"(", 0, 1), Token(NUMBER,"2", 2.0,1), Token(RIGHT_PAREN,")", 0, 1), Token(EOFI,"\0", 0, 1)}, std::make_shared<Grouping>(std::make_shared<Literal>(2.0))) // bracketed NUMBER literal expression: (2)
        ));
