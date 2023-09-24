#include <gmock/gmock.h>
#include <string>

#include "Environment.hpp"

class EnvironmentMock : public Environment {
 public:
    MOCK_METHOD(void, define, (std::string name, std::any value));
    MOCK_METHOD(std::any, get, (const Token &name));
    MOCK_METHOD(void, assign, (const Token &name, std::any value));
};
