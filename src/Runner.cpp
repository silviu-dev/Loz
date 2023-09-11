#include <fstream>
#include <iostream>

#include "Runner.hpp"
#include "Scanner.hpp"

void Runner::run(const std::vector<std::string> &userInput)
{
    switch (userInput.size())
    {
    case 0:
        runPrompt();
    case 1:
        runFile(userInput[0]);
    default:
        std::cout << "too many arguments\n";
    }
}

void Runner::runPrompt()
{
    std::cout << "running in prompt mode\n";
    std::string line;
    while (getline(std::cin, line))
    {
        auto scanner = std::make_shared<Scanner>(line, errorHandler_);
        auto tokens = scanner->scanTokens();
        for (auto i : tokens)
            std::cout << i.toString() << "\n";
        std::cout << ">";
    }
    std::cout << "ending prompt session\n";
}

void Runner::runFile(const std::string &path)
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
        auto scanner = std::make_shared<Scanner>(input, errorHandler_);
        scanner->scanTokens();
        std::cout << input;
    }
    else
    {
        std::cout << "Couldn't open the file!!!";
    }
}
