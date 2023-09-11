#include "ErrorHandler.hpp"
#include "Runner.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    auto errorHandler = std::make_shared<ErrorHandler>();
    auto runner = std::make_shared<Runner>(errorHandler);
    for (int i = 1; i < argc; i++)
    {
        args.push_back(std::string(argv[i]));
    }
    runner->run(args);
}