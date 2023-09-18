#pragma once
#include "Expr.hpp"
#include <vector>

class AstPrinter : public Visitor, public std::enable_shared_from_this<AstPrinter>
{
  public:
    void print(std::shared_ptr<Expr>);
    void visit(std::shared_ptr<Binary>) override;
    void visit(std::shared_ptr<Grouping>) override;
    void visit(std::shared_ptr<Literal>) override;
    void visit(std::shared_ptr<Unary>) override;
    void parenthesize(std::string Name, std::vector<std::shared_ptr<Expr>>);
};