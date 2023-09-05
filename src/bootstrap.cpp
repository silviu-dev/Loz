#include "Runner.hpp"
#include "Scanner.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    auto runner = std::make_shared<Runner>(std::make_shared<Scanner>());
    for (int i = 1; i < argc; i++)
    {
        args.push_back(std::string(argv[i]));
    }
    runner->run(args);
}