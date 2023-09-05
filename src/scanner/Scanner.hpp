#pragma once
#include <string>
#include <vector>

#include "IScanner.hpp"

class Scanner : public IScanner
{
  public:
    std::list<Token> scanTokens(const std::string &) override;
};