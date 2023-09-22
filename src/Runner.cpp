#include <fstream>
#include <iostream>
#include <typeinfo>

#include "Interpreter.hpp"
#include "Parser.hpp"
#include "Runner.hpp"
#include "Scanner.hpp"

void Runner::run(const std::vector<std::string> &userInput)
{
    switch (userInput.size())
    {
    case 0:
        runPrompt();
        break;
    case 1:
        runFile(userInput[0]);
        break;
    default:
        std::cout << "too many arguments\n";
        break;
    }
}

void Runner::runPrompt()
{
    std::cout << "running in prompt mode\n";
    std::string line;
    std::cout << ">";
    while (getline(std::cin, line))
    {
        auto scanner = std::make_shared<Scanner>(line, errorHandler_);
        auto tokens = scanner->scanTokens();
        if (!tokens.empty())
        {
            auto parser = std::make_shared<Parser>(tokens, errorHandler_);
            auto expr = parser->parse();
            if (expr != nullptr)
            {
                auto interpreter = std::make_shared<Interpreter>(errorHandler_);
                interpreter->interpret(expr);
                auto result = interpreter->getResult();
                if (result.type() == typeid(std::string))
                    std::cout << std::any_cast<std::string>(result) << "\n";
                else if (result.type() == typeid(double))
                    std::cout << std::any_cast<double>(result) << "\n";
                else if (result.type() == typeid(bool))
                    std::cout << std::any_cast<bool>(result) << "\n";
            }
        }
        // if (expr != nullptr)
        // {
        //     auto printer = std::make_shared<AstPrinter>();
        //     printer->print(expr);
        // }
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
