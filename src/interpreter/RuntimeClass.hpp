#pragma once
#include "ICallable.hpp"
#include <memory>
#include <string>

struct RuntimeClass : public ICallable, public std::enable_shared_from_this<RuntimeClass>
{
    RuntimeClass(const std::string &name, std::shared_ptr<RuntimeClass> superclass,
                 std::map<std::string, ICallablePtr> &methods)
        : name_(name), superclass_(superclass), methods_(methods)
    {
    }

    std::any call(std::shared_ptr<Interpreter> interpreter, std::vector<std::any> arguments) override;
    int arity() override;
    std::string toString();
    ICallablePtr findMethod(const std::string &name);

  private:
    const std::string name_;
    std::shared_ptr<RuntimeClass> superclass_;
    std::map<std::string, ICallablePtr> methods_;
    friend class RuntimeClassInstance;
};

using RuntimeClassPtr = std::shared_ptr<RuntimeClass>;