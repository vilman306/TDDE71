#ifndef NODE_H
#define NODE_H

#include <string>

class Node
{

public:
    Node() = default;
    virtual ~Node() = default;
    Node(const Node &other) = delete;
    Node &operator=(const Node &other) = delete;

    virtual double evaluate() const = 0;
    virtual std::string postfix() const = 0;
    virtual std::string prefix() const = 0;
    virtual std::string infix() const = 0;
};

#endif