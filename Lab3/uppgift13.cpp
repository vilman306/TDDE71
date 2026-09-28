#include <iostream>
#include <stdexcept>
#include <cstdlib> // std::exit

#include "expression.hpp"

void process_command(const std::string &command, const Expression &e)
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
        std::exit(0);
    else
        throw std::logic_error("Invalid command");
}

int main()
{
    std::string line{};
    Expression e{};

    while (std::getline(std::cin, line))
    {
        try
        {
            if (line.at(0) == ':')
                process_command(line.substr(1), e);
            else
                e.from_infix(line);
        }

        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}
