#include "operand.hpp"

#include <iomanip>
#include <sstream>
#include <string>

std::string Real::postfix() const
{
    std::ostringstream oss;
    oss << std::setprecision(3) << std::fixed << value;
    return oss.str();
}

std::string Integer::postfix() const
{
    std::ostringstream oss;
    oss << std::setprecision(3) << std::fixed << value;
    return oss.str();
}