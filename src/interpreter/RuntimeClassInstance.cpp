#include "RuntimeClassInstance.hpp"
#include "RuntimeFunction.hpp"

std::string RuntimeClassInstance::toString()
{
    return klass_->name_ + " instance";
}

std::any RuntimeClassInstance::get(const Token &name)
{
    auto method = klass_->findMethod(name.lexeme_);
    if (method != nullptr)
        return std::static_pointer_cast<RuntimeFunction>(method)->bind(shared_from_this());

    auto it = fields_.find(name.lexeme_);
    if (it == fields_.end())
    {
        throw Interpreter::InterpreterError(name, std::string("Undefined property ") + name.lexeme_);
    }
    else
    {
        return it->second;
    }
}

void RuntimeClassInstance::set(const Token &name, std::any value)
{
    fields_.insert(std::make_pair(name.lexeme_, value));
}