#include "RuntimeClass.hpp"
#include "RuntimeClassInstance.hpp"
#include "RuntimeFunction.hpp"

std::string RuntimeClass::toString()
{
    return name_;
}

std::any RuntimeClass::call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments)
{
    auto instance = std::make_shared<RuntimeClassInstance>(shared_from_this());

    auto it = methods_.find("init");
    if (it != methods_.end())
    {
        static_pointer_cast<RuntimeFunction>(it->second)->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

int RuntimeClass::arity()
{
    auto it = methods_.find("init");
    if (it != methods_.end())
    {
        return it->second->arity();
    }
    return 0;
}

ICallablePtr RuntimeClass::findMethod(const std::string &name)
{
    auto it = methods_.find(name);
    if (it != methods_.end())
        return it->second;
    if (superclass_ != nullptr)
    {
        return superclass_->findMethod(name);
    }
    return nullptr;
}