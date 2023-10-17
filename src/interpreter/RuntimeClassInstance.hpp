#pragma once
#include "RuntimeClass.hpp"
#include <map>
#include <memory>
#include <string>

struct RuntimeClassInstance : public std::enable_shared_from_this<RuntimeClassInstance>
{
    RuntimeClassInstance(RuntimeClassPtr klass) : klass_(klass)
    {
    }
    std::any get(const Token &name);
    void set(const Token &name, std::any value);
    std::string toString();

  private:
    RuntimeClassPtr klass_;
    std::map<std::string, std::any> fields_;
};

using RuntimeClassInstancePtr = std::shared_ptr<RuntimeClassInstance>;