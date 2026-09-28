#include "postfix.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <istream>

#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

// Public

set<string> const Token::default_operators{
    "+", "-", "*", "/", "%", "^", "=", "(", ")", "?", "Â£", "$"};

string const Token::default_separators{" \t\n\r"};

Token::Token(set<string> const &ops, string const &sep)
    : operators{ops}, separators{sep}, token{}
{
}

istream &operator>>(istream &iss, Token &t)
{
    return t.next(iss);
}

ostream &operator<<(ostream &oss, Token const &t)
{
    return oss << t.token;
}

bool Token::is_operator() const
{
    return operators.count(token) == 1;
}

bool Token::is_integer() const
{
    return all_of(token.begin(), token.end(), ::isdigit);
}

bool Token::is_decimal() const
{
    bool valid_chars = all_of(token.begin(), token.end(), [](char c) -> bool
                              { return c == '.' || isdigit(c); });
    bool one_dot = count(token.begin(), token.end(), '.') == 1;
    return valid_chars && one_dot;
}

bool Token::is_identifier() const
{
    return all_of(token.begin(), token.end(), [](char c) -> bool
                  { return c == '_' || isalnum(c); });
}

// Private

bool Token::is_separator(int c) const
{
    return (separators.find(c) != string::npos);
}

bool Token::is_delimeter(int c) const
{
    if (c == -1) // Traits::eof()
        return true;

    if (is_separator(c))
        return true;

    auto b{operators.begin()};
    auto e{operators.end()};

    for (; b != e; ++b)
    {
        if (b->at(0) == c)
            return true;
    }
    return false;
}

bool Token::is_candidate() const
{
    auto b{operators.begin()};
    auto e{operators.end()};

    for (; b != e; ++b)
    {
        if (b->find(token) == 0 && *b != token)
            return true;
    }
    return false;
}

void Token::append(int c)
{
    token.push_back(static_cast<char>(c));
}

void Token::ignore_separators(istream &iss)
{
    auto c{iss.peek()};

    while (c != -1) // Traits::eof()
    {
        if (!is_separator(c))
            return;

        iss.get();
        c = iss.peek();
    }
}

istream &Token::next(istream &iss)
{
    token.clear();

    ignore_separators(iss);

    auto c{iss.peek()};

    bool prev_is_op{false};

    while (c != -1) // Traits::eof()
    {
        append(c);

        bool is_op{is_operator()};
        bool is_can{is_candidate()};
        bool is_del{is_delimeter(c)};

        if (is_op && !is_can)
        {
            iss.get();
            return iss;
        }

        if (prev_is_op && !is_op && !is_can)
        {
            token.pop_back();
            return iss;
        }

        if (is_del && !is_can)
        {
            token.pop_back();
            return iss;
        }

        prev_is_op = is_op;
        iss.get();
        c = iss.peek();
    }

    return iss;
}

//=============================================

// Public

Postfix::Postfix(string const &infix_string) : expr{}
{
    istringstream is{infix_string};
    expr = make_postfix(is);
}

string Postfix::to_string() const
{
    ostringstream os;
    copy(begin(expr), end(expr),
         ostream_iterator<string>{os, " "});
    return os.str();
}

Postfix::operator string() const
{
    return to_string();
}

bool Postfix::operator==(string const &rhs) const
{
    return to_string() == rhs;
}

// Private

// right associative operators have input priority > stack priority
// left associative operators have input priority < stack priority
const Postfix::priority_table Postfix::operator_table{
    // {symbol, input prio, stack prio}
    {"^", {8, 7}},
    {"*", {5, 6}},
    {"/", {5, 6}},
    {"%", {5, 6}},
    {"+", {3, 4}},
    {"-", {3, 4}},
    {"=", {2, 1}}};

bool Postfix::is_operator(const string &token)
{
    // C++20: return operator_table.contains( token );
    return (operator_table.count(token) > 0);
}

Postfix::expression Postfix::make_postfix(istream &is, bool match_parenthesis) const
{
    using namespace literals; // for string literals ""s

    op_stack operator_stack;
    expression postfix;
    Token token;
    int unused_operands{0};
    bool implicit_mult_possible{false};

    // Initialize handle_precedence ...
    auto handle_precedence =
        // ... with an anonymous class instance ...
        [
            // ... created with three reference data members:
            &operator_stack, &postfix, &unused_operands]
        // ... and operator()( ... ) implemented ...
        (string const &tok)
    {
        while (!operator_stack.empty() &&
               operator_table.at(tok).input <=
                   operator_table.at(operator_stack.top()).stack)
        {
            postfix.push_back(operator_stack.top());
            unused_operands -= 1; // uses 2 but creates 1
            operator_stack.pop();
        }
        operator_stack.push(tok);
    }; // ... making it callable below like a normal function!

    while (is >> token && token != ")"s)
    {
        if (is_operator(token))
        {
            handle_precedence(token);
        }
        else if (token == "("s)
        {
            if (unused_operands - operator_stack.size() >= 1)
            {
                handle_precedence("*"); // add implicit multiplication
            }
            expression parentesis{make_postfix(is, true)};
            copy(begin(parentesis), end(parentesis), back_inserter(postfix));
            unused_operands += 1; // entire postix is 1 operand
            implicit_mult_possible = true;
        }
        else
        {
            if (unused_operands - operator_stack.size() >= 1)
            {
                if (implicit_mult_possible)
                {
                    handle_precedence("*"); // add implicit multiplication
                    implicit_mult_possible = false;
                }
                else
                {
                    throw Infix_Error{"Two operands in a row"};
                }
            }
            postfix.push_back(token);
            unused_operands += 1;
        }
    }

    if (postfix.empty() && match_parenthesis && token == ")"s)
    {
        throw Infix_Error{"Empty parenthesis"};
    }

    if (match_parenthesis && token != ")"s)
    {
        throw Infix_Error{"Missing ending parenthesis"};
    }

    if (!match_parenthesis && token == ")"s)
    {
        throw Infix_Error{"Missing starting parenthesis"};
    }

    while (!operator_stack.empty())
    {
        postfix.push_back(operator_stack.top());
        unused_operands -= 1; // uses 2 but creates 1
        operator_stack.pop();
    }

    if (unused_operands != 1) // the last one is the answer
    {
        if (unused_operands > 1)
            throw Infix_Error{"Missing operator"};
        else
            throw Infix_Error{"Missing operand"};
    }

    return postfix;
}
