#ifndef POSTFIX_H
#define POSTFIX_H

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <istream>
#include <stdexcept>
#include <set>

// Creates a Postfix from a given infix string. All operands and
// operators are required to be separated by at least one space.
class Postfix
{
public:
    Postfix(std::string const &infix_string);

    std::string to_string() const;
    operator std::string() const;
    bool operator==(std::string const &rhs) const;

private:
    using op_stack = std::stack<std::string>;
    using expression = std::vector<std::string>;

    expression expr;

    struct priority
    {
        int input;
        int stack;
    };

    using priority_table = std::map<std::string, Postfix::priority>;

    static const priority_table operator_table;

    static bool is_operator(const std::string &token);

    expression make_postfix(std::istream &is, bool match_parenthesis = false) const;
};

//======================

// A class to read strings from an istream just like std::string,
// but tokens are separated not only by space, but also by operators.
//
// You should be able to just change from "std::string" to "Token" in
// your program unless you do something special with your strings.
//
// Tokens can be classified as integers, deciamls, identifiers or operators.
//
class Token
{
public:
    // interoperability with standard library (STL)
    using token_t = std::string;

    using value_type = token_t::value_type;
    using size_type = token_t::size_type;
    using pointer = token_t::pointer;
    using reference = token_t::reference;
    using iterator = token_t::const_iterator;

    iterator begin() const { return token.begin(); }
    iterator end() const { return token.end(); }
    size_type size() const { return token.size(); }
    size_type length() const { return token.length(); }
    value_type at(int i) const { return token.at(i); }

    // static constants for default values
    static std::set<std::string> const default_operators;
    static std::string const default_separators;

    // public members
    Token(std::set<std::string> const &operators = default_operators,
          std::string const &separators = default_separators);

    friend std::istream &operator>>(std::istream &iss, Token &t);

    friend std::ostream &operator<<(std::ostream &oss, Token const &t);

    operator std::string() const { return token; }

    bool operator==(std::string const &rhs) { return token == rhs; }
    bool operator!=(std::string const &rhs) { return token != rhs; }

    bool is_operator() const;
    bool is_integer() const;
    bool is_decimal() const;
    bool is_identifier() const;

private:
    // help functions
    bool is_delimeter(int c) const;
    bool is_separator(int c) const;

    bool is_candidate() const;

    void append(int c);
    void ignore_separators(std::istream &iss);

    std::istream &next(std::istream &iss);

    // data members
    std::set<std::string> const &operators;
    std::string const &separators;
    std::string token;

    friend class Token_Private_Tester;
};

class Infix_Error : public std::logic_error
{
    using std::logic_error::logic_error;
};

#endif
