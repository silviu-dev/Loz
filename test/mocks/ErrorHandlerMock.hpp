#include <gmock/gmock.h>

#include "IErrorHandler.hpp"

class ErrorHandlerMock : public IErrorHandler {
 public:
  MOCK_METHOD(void, signalError, (int line,const std::string& message));
};