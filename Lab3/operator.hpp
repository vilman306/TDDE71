#ifndef OPERATOR_H
#define OPERATOR_H

#include "node.hpp"
#include "operand.hpp"

#include <cmath>

class Operator : public Node
{
public:
    std::string postfix() const override;
    std::string prefix() const override;
    std::string infix() const override;

    enum signs
    {
        addition = '+',
        subtraction = '-',
        multiplication = '*',
        division = '/',
        power = '^',
        modulo = '%',
        condition = '?'
    };

protected:
    Operator(
        const char _sign,
        Node *_child_left,
        Node *_child_right) : sign{_sign}, child_left{_child_left}, child_right{_child_right} {}

    char sign;
    Node *child_left;
    Node *child_right;
};

class Addition : public Operator
{
public:
    Addition(Node *_child_left, Node *_child_right) : Operator{Operator::signs::addition, _child_left, _child_right} {}

    double evaluate() const override
    {
        return child_left->evaluate() + child_right->evaluate();
    }
};

class Subtraction : public Operator
{
public:
    Subtraction(Node *_child_left, Node *_child_right) : Operator{Operator::signs::subtraction, _child_left, _child_right} {}

    double evaluate() const override
    {
        return child_left->evaluate() - child_right->evaluate();
    }
};

class Multiplication : public Operator
{
public:
    Multiplication(Node *_child_left, Node *_child_right) : Operator{Operator::signs::multiplication, _child_left, _child_right} {}

    double evaluate() const override
    {
        return child_left->evaluate() * child_right->evaluate();
    }
};

class Division : public Operator
{
public:
    Division(Node *_child_left, Node *_child_right) : Operator{Operator::signs::division, _child_left, _child_right} {}

    double evaluate() const override;
};

class Power : public Operator
{
public:
    Power(Node *_child_left, Node *_child_right) : Operator{Operator::signs::power, _child_left, _child_right} {}

    double evaluate() const override;
};

class Modulo : public Operator
{
public:
    Modulo(Node *_child_left, Node *_child_right) : Operator{Operator::signs::modulo, _child_left, _child_right} {}

    double evaluate() const override { return fmod(child_left->evaluate(), child_right->evaluate()); }
};

class Condition : public Operator
{
public:
    Condition(Node *_child_left, Node *_child_right) : Operator{Operator::signs::condition, _child_left, _child_right} {}

    double evaluate() const override;
};
#endif