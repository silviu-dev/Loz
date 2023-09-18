#include <gmock/gmock.h>

#include "IErrorHandler.hpp"

class ErrorHandlerMock : public IErrorHandler {
 public:
    MOCK_METHOD(void, error, (Token token,const std::string& message));
    MOCK_METHOD(void, error, (int line,const std::string& message));
};