#pragma once
#include "Stmt.hpp"
#include <vector>

class AstPrinter : public StmtVisitor, public ExprVisitor, public std::enable_shared_from_this<AstPrinter>
{
  public:
    void print(std::shared_ptr<Stmt> stmt);
    std::any visit(std::shared_ptr<Expression> expression) override;
    std::any visit(std::shared_ptr<Print> print) override;
    std::any visit(std::shared_ptr<Var>) override;
    std::any visit(std::shared_ptr<Binary>) override;
    std::any visit(std::shared_ptr<Grouping>) override;
    std::any visit(std::shared_ptr<Literal>) override;
    std::any visit(std::shared_ptr<Unary>) override;
    std::any visit(std::shared_ptr<Variable>) override;
    void parenthesize(std::string Name, std::vector<std::shared_ptr<Expr>>);
    void printValue(std::any value);
};