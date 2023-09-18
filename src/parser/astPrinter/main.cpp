#include "AstPrinter.hpp"
int main()
{
    std::shared_ptr<Expr> expression = std::make_shared<Binary>(
        std::make_shared<Unary>(
            std::make_shared<Token>(MINUS, "-", nullptr, 1),
            std::make_shared<Literal>(std::make_shared<float>(123), std::make_shared<TokenType>(NUMBER))),
        std::make_shared<Token>(STAR, "*", nullptr, 1),
        std::make_shared<Grouping>(
            std::make_shared<Literal>(std::make_shared<float>(45.67), std::make_shared<TokenType>(NUMBER))));
    auto printer = std::make_shared<AstPrinter>();
    printer->print(expression);
}
