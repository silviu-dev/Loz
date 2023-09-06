#pragma once
#include <memory>
#include <string>
#include <vector>

#include "IErrorHandler.hpp"
#include "IScanner.hpp"

class Runner
{
  public:
    Runner(const IScannerPtr &scanner, const IErrorHandlerPtr &errorHandler)
        : scanner_(scanner), errorHandler_(errorHandler){};
    int run(std::vector<std::string>);

  private:
    int runPrompt();
    int runFile(const std::string &path);
    IScannerPtr scanner_;
    IErrorHandlerPtr errorHandler_;
};

using RunnerPtr = std::shared_ptr<Runner>;