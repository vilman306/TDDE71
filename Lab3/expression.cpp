#include "expression.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stack>

#include "node.hpp"
#include "operator.hpp"
#include "operand.hpp"
#include "postfix.hpp"

void Expression::from_infix(const std::string &infix)
{
    Postfix postfix{infix};
    std::istringstream iss{postfix.to_string()};
    std::string current{};
    std::stack<Node *> stack{};
    while (iss >> current)
    {
        if (std::all_of(begin(current), end(current), ::isdigit))
        {
            // Vi har hittat ett heltal
            int a = std::stoi(current);
            stack.push(new Integer{a});
        }
        else if (isdigit(current.at(0)))
        {
            // Vi hoppas ordet är ett flyttal
            double a = std::stod(current);
            stack.push(new Real{a});
        }
        else
        {
            // Vi hoppas ordet är en operator
            if (stack.size() < 2)
            {
                throw std::logic_error("missing operands for operator");
            }

            Operator *a{};

            Node *r{stack.top()};
            stack.pop();

            Node *l{stack.top()};
            stack.pop();

            switch (current.at(0))
            {
            case Operator::signs::addition:
                a = new Addition{l, r};
                break;
            case Operator::signs::subtraction:
                a = new Subtraction{l, r};
                break;
            case Operator::signs::multiplication:
                a = new Multiplication{l, r};
                break;
            case Operator::signs::division:
                a = new Division{l, r};
                break;
            case Operator::signs::power:
                a = new Power{l, r};
                break;
            case Operator::signs::modulo:
                a = new Modulo{l, r};
                break;
            case Operator::signs::condition:
                a = new Condition{l, r};
                break;
            default:
                throw std::logic_error("undefined operator");
            }

            stack.push(a);
        }
    }
    if (stack.size() != 1)
        throw std::logic_error("stack has 0 or more than 1 element "
                               "at end of Expression constructor");
    root = stack.top();
}

std::string Expression::process_command(const std::string &command) const
{
    if (command == "calc")
        return std::to_string(evaluate());
    else
        throw std::logic_error("Invalid command");

}

std::string Expression::to_postfix() const
{
    return root->postfix();
}

std::string Expression::to_prefix() const
{
    return root->prefix();
}

std::string Expression::to_infix() const
{
    return root->infix();
}

double Expression::evaluate() const
{
    return root->evaluate();
}