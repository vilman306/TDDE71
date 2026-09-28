#ifndef OPERAND_H
#define OPERAND_H

#include "node.hpp"

class Operand : public Node
{
    // empty class, woohoo
};

class Real : public Operand
{
public:
    Real(const double _value) : value{_value} {}

    double evaluate() const override
    {
        return value;
    }

    std::string postfix() const override;
    std::string prefix() const override { return postfix(); }
    std::string infix() const override { return postfix(); }

protected:
    const double value;
};

class Integer : public Operand
{
public:
    Integer(const int _value) : value{_value} {}

    double evaluate() const override
    {
        return value;
    }

    std::string postfix() const override;
    std::string prefix() const override { return postfix(); }
    std::string infix() const override { return postfix(); }

protected:
    const int value;
};

#endif