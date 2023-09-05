#pragma once
#include <memory>
#include <string>
#include <vector>

#include "IScanner.hpp"

class Runner
{
  public:
    Runner(const IScannerPtr &scanner) : scanner_(scanner){};
    int run(std::vector<std::string>);

  private:
    int runPrompt();
    int runFile();
    IScannerPtr scanner_;
};

using RunnerPtr = std::shared_ptr<Runner>;