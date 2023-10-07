#include "Resolver.hpp"

std::any Resolver::visit(std::shared_ptr<Expression> expression)
{
    resolve(expression->expression);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Print> print)
{
    resolve(print->expression);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Var> declaration)
{
    declare(declaration->name);
    if (declaration->initializer != nullptr)
    {
        resolve(declaration->initializer);
    }
    define(declaration->name);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<While> stmt)
{
    resolve(stmt->condition);
    resolve(stmt->body);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Block> block)
{
    beginScope();
    resolve(block->statements);
    endScope();
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<If> stmt)
{
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr)
        resolve(stmt->elseBranch);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Assign> assign)
{
    resolve(assign->value);
    resolveLocal(assign, assign->name);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Logical> expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Binary> binary)
{
    resolve(binary->left);
    resolve(binary->right);
    return nullptr;
}
std::any Resolver::visit(std::shared_ptr<Grouping> grouping)
{
    resolve(grouping->expression);
    return nullptr;
}
std::any Resolver::visit(std::shared_ptr<Literal> literal)
{
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Call> expr)
{
    for (auto argument : expr->arguments)
    {
        resolve(argument);
    }
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Return> ret)
{
    if (ret->value != nullptr)
    {
        resolve(ret->value);
    }
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Function> stmt) // function declaration
{
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Unary> unary)
{
    resolve(unary->right);
    return nullptr;
}

std::any Resolver::visit(std::shared_ptr<Variable> variable)
{
    if (!scopes.empty())
    {
        const auto &elem = (scopes.end() - 1)->find(variable->name.lexeme_);
        if (elem != (scopes.end() - 1)->end())
        {
            if (elem->second == false)
            {
                errorHandler_->error(variable->name, "Can't read local variable in its own initializer.");
            }
        }
    }
    resolveLocal(variable, variable->name);
    return nullptr;
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements)
{
    for (auto statement : statements)
    {
        resolve(statement);
    }
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt)
{
    stmt->accept(shared_from_this());
}

void Resolver::resolve(std::shared_ptr<Expr> expr)
{
    expr->accept(shared_from_this());
}

void Resolver::beginScope()
{
    scopes.push_back(std::map<std::string, bool>());
}

void Resolver::endScope()
{
    scopes.erase(scopes.end() - 1);
}

void Resolver::declare(const Token &name)
{
    if (scopes.empty())
        return;
    auto &scope = *(scopes.end() - 1);
    scope.insert(std::pair<std::string, bool>(name.lexeme_, false));
}

void Resolver::define(const Token &name)
{
    if (scopes.empty())
        return;
    auto &scope = *(scopes.end() - 1);
    auto it = scope.find(name.lexeme_);
    it->second = true;
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, const Token &name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].find(name.lexeme_) != scopes[i].end())
        {
            interpreter_->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(std::shared_ptr<Function> function)
{
    beginScope();
    for (auto param : function->params)
    {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
}