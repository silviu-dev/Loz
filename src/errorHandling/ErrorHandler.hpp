#include "IErrorHandler.hpp"

struct ErrorHandler : public IErrorHandler
{
    void error(Token token, const std::string &message) override;
    void error(int line, const std::string &message) override;

  private:
    void report(int line, std::string where, std::string message);
};