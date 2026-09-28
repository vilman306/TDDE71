#include "operator.hpp"

#include <stdexcept>

std::string Operator::postfix() const
{
    return child_left->postfix() + " " + child_right->postfix() + " " + sign;
}

std::string Operator::prefix() const
{
    return std::string{sign} + " " + child_left->prefix() + " " + child_right->prefix();
}

std::string Operator::infix() const
{
    return "( " + child_left->infix() + " " + sign + " " + child_right->infix() + " )";
}

double Division::evaluate() const
{
    double right_side = child_right->evaluate();

    if (right_side == 0.0)
    {
        throw std::logic_error("division by zero not allowed");
    }

    return child_left->evaluate() / child_right->evaluate();
}

double Power::evaluate() const
{
    double left_side = child_left->evaluate();
    double right_side = child_right->evaluate();

    Real *casted_right = dynamic_cast<Real *>(child_right);

    if (left_side < 0 && casted_right != nullptr)
    {
        throw std::logic_error("negative base with floating point exponent not allowed");
    }
    else if (left_side == 0 && right_side < 0)
    {
        throw std::logic_error("zero as base with negative exponent not allowed");
    }

    return pow(left_side, right_side);
}

double Condition::evaluate() const
{
    double left_side = child_left->evaluate();

    return left_side != 0.0 ? child_right->evaluate() : 0.0;
};