#include "Time.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>


// Utility functions used in Time's member functions
// ============================

void validate_time_in_bounds(TimeData time)
{
    if (time.Hour > Time::s_MaxHour)
    {
        throw std::out_of_range(std::string("Hour must be in interval [0, ") +
                                std::to_string(Time::s_MaxHour) + "]");
    }
    if (time.Minute > Time::s_MaxMinute)
    {
        throw std::out_of_range(std::string("Minute must be in interval [0, ") +
                                std::to_string(Time::s_MaxMinute) + "]");
    }
    if (time.Second > Time::s_MaxSecond)
    {
        throw std::out_of_range(std::string("Second must be in interval [0, ") +
                                std::to_string(Time::s_MaxSecond) + "]");
    }
    if (time.Millisecond > Time::s_MaxMillisecond)
    {
        throw std::out_of_range(std::string("Second must be in interval [0, ") +
                                std::to_string(Time::s_MaxMillisecond) + "]");
    }
}

void throw_invalid_string_format()
{
    throw std::logic_error("String argument has invalid format. "
                           "Expected format is HH:MM:SS or HH:MM:SS.mmm");
}



// Definitions of class member functions:
// =====================================

Time::Time(unsigned int hour, unsigned int minute, unsigned int second)
    : m_Time{hour, minute, second}
{
    validate_time_in_bounds(m_Time);
}

Time::Time(unsigned int hour, unsigned int minute, unsigned int second,
           unsigned int millisecond)
    : m_Time{hour, minute, second, millisecond}
{
    validate_time_in_bounds(m_Time);
}

Time::Time(const std::string& s)
    : m_Time{}
{
    // Extract string data
    // ===================
    std::istringstream iss{s};
    int hour{}, minute{}, second{}, millisecond{};
    int itemp{};
    char ctemp{};
    
    // (Expected format is HH:MM:SS.mmm or HH:MM:SS)
    // (H:M:S.m also works)
    // Get hour
    if (iss >> itemp)
        hour = itemp;
    else
        throw_invalid_string_format();
    
    // Check for colon
    if (!(iss >> ctemp && ctemp == ':'))
        throw_invalid_string_format();
    
    // Get minute
    if (iss >> itemp)
        minute = itemp;
    else
        throw_invalid_string_format();
    
    // Check for colon
    if (!(iss >> ctemp && ctemp == ':'))
        throw_invalid_string_format();
    
    // Get second
    if (iss >> itemp)
        second = itemp;
    else
        throw_invalid_string_format();

    // Check if any data is negative
    if (hour < 0 || minute < 0 || second < 0)
    {
        throw std::out_of_range("String argument contains negative integer(s)");
    }

    // Handle milliseconds:
    bool has_millisecond = !iss.eof();
    if (has_millisecond)
    {
        // Check for dot
        if (!(iss >> ctemp && ctemp == '.'))
            throw_invalid_string_format();

        // Get millisecond
        if (iss >> itemp)
            millisecond = itemp;
        else
            throw_invalid_string_format();

        // Throw if there are characters after the milliseconds
        if (!iss.eof())
            throw_invalid_string_format();

        // Throw if millisecond data is negative
        if (millisecond < 0)
            throw std::out_of_range("String argument contains negative integer(s)");
    }

    // Update the member variables
    m_Time.Hour = hour; // implicitly cast from int to unsigned int
    m_Time.Minute = minute;
    m_Time.Second = second;
    m_Time.Millisecond = millisecond;

    validate_time_in_bounds(m_Time);
}

std::string Time::ToString(bool twelve_hour) const
{
    std::ostringstream oss{};
    unsigned int hour = m_Time.Hour;

    if (twelve_hour)
        hour = ((hour + 11) % 12) + 1;

    oss << std::setfill('0')
        << std::right
        << std::setw(2)
        << hour
        << ':'
        << std::setw(2)
        << m_Time.Minute
        << ':'
        << std::setw(2)
        << m_Time.Second;

    if (m_Time.Millisecond != 0)
        oss << '.'
            << std::setw(3)
            << m_Time.Millisecond;

    if (twelve_hour)
        oss << (IsAM() ? "am" : "pm");

    return oss.str();
}

bool Time::IsAM() const
{
    return (m_Time.Hour < 12);
}

float Time::GetSeconds() const
{
    return m_Time.Hour * 60.0f * 60.0f + m_Time.Minute * 60.0f +
           m_Time.Second + m_Time.Millisecond * 0.001f;
}

void Time::Increment()
{
    // Increase or reset second
    m_Time.Second = (m_Time.Second == s_MaxSecond) ? 0 : m_Time.Second + 1;
    if (m_Time.Second != 0)
        return;
    // Increase or reset minute if second was max
    m_Time.Minute = (m_Time.Minute == s_MaxMinute) ? 0 : m_Time.Minute + 1;
    if (m_Time.Minute != 0)
        return;
    // Increase or reset hour if minute and second was max
    m_Time.Hour = (m_Time.Hour == s_MaxHour) ? 0 : m_Time.Hour + 1;
}








// Operator overloads:
// ===================


// Prefix increment
Time& Time::operator++()
{
    Increment();
    return *this;
}

// Postfix increment
Time Time::operator++(int)
{
    Time prev_time = *this;
    Increment();
    return prev_time;
}

bool operator<(const Time& left, const Time& right)
{
    return left.GetSeconds() < right.GetSeconds();
}

bool operator>(const Time& left, const Time& right)
{
    return left.GetSeconds() > right.GetSeconds();
}

bool operator<=(const Time& left, const Time& right)
{
    return left.GetSeconds() <= right.GetSeconds();
}

bool operator>=(const Time& left, const Time& right)
{
    return left.GetSeconds() >= right.GetSeconds();
}

bool operator==(const Time& left, const Time& right)
{
    return left.GetSeconds() == right.GetSeconds();
}

bool operator!=(const Time& left, const Time& right)
{
    return left.GetSeconds() != right.GetSeconds();
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
    os << time.ToString();
    return os;
}

float operator-(const Time& left, const Time& right)
{
    return left.GetSeconds() - right.GetSeconds();
}