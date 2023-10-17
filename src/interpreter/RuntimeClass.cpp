#include "RuntimeClass.hpp"
#include "RuntimeClassInstance.hpp"

std::string RuntimeClass::toString()
{
    return name_;
}

std::any RuntimeClass::call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments)
{
    return std::make_shared<RuntimeClassInstance>(shared_from_this());
}

int RuntimeClass::arity()
{
    return 0;
}

ICallablePtr RuntimeClass::findMethod(const std::string &name)
{
    auto it = methods_.find(name);
    if (it != methods_.end())
        return it->second;
    return nullptr;
}