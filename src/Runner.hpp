#pragma once
#include <memory>
#include <string>
#include <vector>

#include "IErrorHandler.hpp"
#include "IScanner.hpp"

class Runner
{
  public:
    Runner(const IErrorHandlerPtr &errorHandler) : errorHandler_(errorHandler){};
    void run(const std::vector<std::string> &userInput);

  private:
    void runPrompt();
    void runFile(const std::string &path);
    IErrorHandlerPtr errorHandler_;
};

using RunnerPtr = std::shared_ptr<Runner>;