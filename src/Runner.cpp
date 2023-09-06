#include "Runner.hpp"
#include <fstream>
#include <iostream>

int Runner::run(std::vector<std::string> args)
{
    // scanner_->scanTokens(args);
    switch (args.size())
    {
    case 0:
        return runPrompt();
    case 1:
        return runFile(args[0]);
    default:
        std::cout << "too many arguments\n";
        return -1;
    }
}

int Runner::runPrompt()
{
    std::cout << "running in prompt mode\n";
    std::string line;
    while (getline(std::cin, line))
    {
        scanner_->scanTokens(line);
        std::cout << "\n>";
    }
    std::cout << "ending prompt session\n";
    return 0;
}

int Runner::runFile(const std::string &path)
{
    std::ifstream f(path);
    std::string input, line;
    std::cout << "running in file mode\n";
    if (f.is_open())
    {
        while (getline(f, line))
        {
            input += line;
        }
        scanner_->scanTokens(input);
        std::cout << input;
    }
    else
    {
        std::cout << "Couldn't open the file!!!";
        return -1;
    }
    return 0;
}
