#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
// #include <cstdlib> // std::exit

#include "expression.hpp"

std::vector<Expression> saved_expressions{};

// Returns false if program should quit
bool process_command(const std::string &command, Expression &e)
{
    if (command == "calc")
        std::cout << e.evaluate() << std::endl;
    else if (command == "postfix")
        std::cout << e.to_postfix() << std::endl;
    else if (command == "prefix")
        std::cout << e.to_prefix() << std::endl;
    else if (command == "infix")
        std::cout << e.to_infix() << std::endl;
    else if (command == "quit")
        return false;
        // std::exit(0); // If you use this, the stack allocated variables' destructors won't run
    else if (command == "save") {
        saved_expressions.push_back(Expression{e.to_infix()});
    }
    else if (command == "list") {
        for (unsigned int i{1}; i <= saved_expressions.size(); i++)
        {
            std::cout << i << ": " << saved_expressions.at(i-1).to_infix() << "\n";
        }
        std::cout << std::flush;
    }
    else
    {
        std::string s;
        std::istringstream iss{command};
        iss >> s;
        if (s == "activate")
        {
            int index{};

            if (iss.eof())
                throw std::logic_error("No index inputted ");
                
            iss >> index;
            if (iss.fail())
                throw std::logic_error("Please input a number!");

            else if (static_cast<unsigned int>(index) > saved_expressions.size() || index < 1)
                throw std::logic_error("There is no expression at index " + std::to_string(index));
            
            
            e.from_postfix(saved_expressions.at(index - 1).to_postfix());
        }
        else
            throw std::logic_error("Invalid command");
    }

    return true;
}

int main()
{
    std::string line{};
    Expression e{};

    while (std::getline(std::cin, line))
    {
        try
        {
            if (line.empty())
                continue;

            if (line.at(0) == ':') // If input is command
            {
                if (!process_command(line.substr(1), e))
                    break; // command was :quit
            }
            else
            {
                e.from_infix(line);
            }
        }

        catch (const std::exception &exception)
        {
            std::cerr << exception.what() << "\n";
        }
    }

    return 0;
}
