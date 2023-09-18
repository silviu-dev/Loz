#pragma once
#include "Expr.hpp"
#include <any>
#include <vector>

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>
{
  public:
    void interpret(std::shared_ptr<Expr>);
    void visit(std::shared_ptr<Binary>) override;
    void visit(std::shared_ptr<Grouping>) override;
    void visit(std::shared_ptr<Literal>) override;
    void visit(std::shared_ptr<Unary>) override;
    std::any evaluate(std::shared_ptr<Expr> expr);
    bool isTruthy(std::any object);
    std::any getResult()
    {
        return result_;
    }

  private:
    std::any result_;
};