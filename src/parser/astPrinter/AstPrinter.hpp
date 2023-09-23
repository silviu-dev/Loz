#pragma once
#include "Stmt.hpp"
#include <vector>

class AstPrinter : public StmtVisitor, public ExprVisitor, public std::enable_shared_from_this<AstPrinter>
{
  public:
    void print(std::shared_ptr<Stmt> stmt);
    void visit(std::shared_ptr<Expression> expression) override;
    void visit(std::shared_ptr<Print> print) override;
    void visit(std::shared_ptr<Var>) override;
    void visit(std::shared_ptr<Binary>) override;
    void visit(std::shared_ptr<Grouping>) override;
    void visit(std::shared_ptr<Literal>) override;
    void visit(std::shared_ptr<Unary>) override;
    void visit(std::shared_ptr<Variable>) override;
    void parenthesize(std::string Name, std::vector<std::shared_ptr<Expr>>);
};