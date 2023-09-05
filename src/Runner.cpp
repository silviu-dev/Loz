#include "Runner.hpp"
#include <iostream>

int Runner::run(std::vector<std::string> args)
{
    // scanner_->scanTokens(args);
    switch (args.size())
    {
    case 0:
        return runPrompt();
    case 1:
        return runFile();
    default:
        std::cout << "too many arguments\n";
        return -1;
    }
}

int Runner::runPrompt()
{
    std::cout << "running in prompt mode\n";
    std::string line;
    std::cout << "> ";
    while (getline(std::cin, line))
    {
        scanner_->scanTokens(line);
        std::cout << "\n>";
    }
    std::cout << "ending prompt session\n";
    return 0;
}

int Runner::runFile()
{
    std::cout << "running in file mode\n";
    return 0;
}