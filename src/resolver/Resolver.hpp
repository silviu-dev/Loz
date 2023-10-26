#include "IErrorHandler.hpp"
#include "Interpreter.hpp"
#include "Stmt.hpp"

class Resolver : public ExprVisitor, public StmtVisitor, public std::enable_shared_from_this<Resolver>
{
  public:
    Resolver(std::shared_ptr<Interpreter> interpreter, const std::shared_ptr<IErrorHandler> &errorHandler)
        : interpreter_(interpreter), errorHandler_(errorHandler)
    {
    }

    std::any visit(std::shared_ptr<Class> cl) override;
    std::any visit(std::shared_ptr<Expression> expression) override;
    std::any visit(std::shared_ptr<Print> print) override;
    std::any visit(std::shared_ptr<Var>) override;
    std::any visit(std::shared_ptr<While> stmt) override;
    std::any visit(std::shared_ptr<Block> block) override;
    std::any visit(std::shared_ptr<If> stmt) override;
    std::any visit(std::shared_ptr<Assign> assign) override;
    std::any visit(std::shared_ptr<Logical> expr) override;
    std::any visit(std::shared_ptr<Binary>) override;
    std::any visit(std::shared_ptr<Grouping>) override;
    std::any visit(std::shared_ptr<Literal>) override;
    std::any visit(std::shared_ptr<Unary>) override;
    std::any visit(std::shared_ptr<Call> expr) override;
    std::any visit(std::shared_ptr<Get> expr) override;
    std::any visit(std::shared_ptr<Set> expr) override;
    std::any visit(std::shared_ptr<Return> ret) override;
    std::any visit(std::shared_ptr<Function> stmt) override;
    std::any visit(std::shared_ptr<Variable>) override;
    std::any visit(std::shared_ptr<This> expr) override;
    std::any visit(std::shared_ptr<Super> super) override;
    void resolve(std::vector<std::shared_ptr<Stmt>> statements);
    void resolve(std::shared_ptr<Stmt> stmt);
    void resolve(std::shared_ptr<Expr> expr);
    void beginScope();
    void endScope();
    void declare(const Token &name);
    void define(const Token &name);
    void resolveLocal(std::shared_ptr<Expr> expr, const Token &name);
    void resolveFunction(std::shared_ptr<Function> function);

    bool panicMode = false;

  private:
    bool inFunction_ = false;
    bool inClass_ = false;
    bool inCtor_ = false;
    bool hasSuperclass_ = false;
    std::shared_ptr<Interpreter> interpreter_;
    std::vector<std::map<std::string, bool>> scopes;
    std::shared_ptr<IErrorHandler> errorHandler_ = nullptr;
};