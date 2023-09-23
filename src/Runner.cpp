#include <fstream>
#include <iostream>
#include <typeinfo>

#include "AstPrinter.hpp"
#include "Environment.hpp"
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
    auto environment = std::make_shared<Environment>();
    auto interpreter = std::make_shared<Interpreter>(errorHandler_, environment);
    while (getline(std::cin, line))
    {
        auto scanner = std::make_shared<Scanner>(line, errorHandler_);
        auto tokens = scanner->scanTokens();

        // for (auto t : tokens)
        // {
        //     std::cout << t.toString() << "\n";
        // }
        if (!tokens.empty())
        {
            auto parser = std::make_shared<Parser>(tokens, errorHandler_);
            auto expr = parser->parse();
            if (!expr.empty())
            {
                interpreter->interpret(expr);
            }
            // auto printer = std::make_shared<AstPrinter>();
            // for (auto e : expr)
            // {
            //     printer->print(e);
            //     std::cout << "\n";
            // }
        }
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
            input += line + '\n';
        }
        auto environment = std::make_shared<Environment>();
        auto scanner = std::make_shared<Scanner>(input, errorHandler_);
        auto tokens = scanner->scanTokens();
        if (!tokens.empty())
        {
            // for (auto t : tokens)
            // {
            //     std::cout << t.toString() << "\n";
            // }
            auto parser = std::make_shared<Parser>(tokens, errorHandler_);
            auto expr = parser->parse();
            if (!expr.empty())
            {
                auto interpreter = std::make_shared<Interpreter>(errorHandler_, environment);
                interpreter->interpret(expr);
            }
            // auto printer = std::make_shared<AstPrinter>();
            // for (auto e : expr)
            // {
            //     printer->print(e);
            //     std::cout << "\n";
            // }
        }
        std::cout << "exiting from file mode\n";
    }
    else
    {
        std::cout << "Couldn't open the file!!!";
    }
}
