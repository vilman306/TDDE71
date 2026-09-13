#ifndef TIME_H
#define TIME_H

#include <string>

struct TimeData
{
    unsigned int Hour{0};
    unsigned int Minute{0};
    unsigned int Second{0};
    unsigned int Millisecond{0};
};

class Time
{
public:
    Time(unsigned int hour, unsigned int minute, unsigned int second);
    Time(unsigned int hour, unsigned int minute, unsigned int second,
         unsigned int millisecond);
    Time(const std::string& s);
    Time() : m_Time{} {}
    
    Time& operator++(); // Prefix increment
    Time operator++(int); // Postfix increment
    
    std::string ToString(bool twelve_hour = false) const;
    bool IsAM() const;
    unsigned int GetHour() const { return m_Time.Hour; }
    unsigned int GetMinute() const { return m_Time.Minute; }
    unsigned int GetSecond() const { return m_Time.Second; }
    unsigned int GetMillisecond() const { return m_Time.Millisecond; }
    
    float GetSeconds() const; // Used in operator overloads

    // (inline prevents multiple definition error)
    inline static const unsigned int s_MaxHour{23};
    inline static const unsigned int s_MaxMinute{59};
    inline static const unsigned int s_MaxSecond{59};
    inline static const unsigned int s_MaxMillisecond{999};
    
    
private:
    void Increment();

    TimeData m_Time;
};

// Forward declations of non-member operator overloads
bool operator<(const Time& left, const Time& right);
bool operator>(const Time& left, const Time& right);
bool operator<=(const Time& left, const Time& right);
bool operator>=(const Time& left, const Time& right);
bool operator==(const Time& left, const Time& right);
bool operator!=(const Time& left, const Time& right);
std::ostream& operator<<(std::ostream& os, const Time& time);
float operator-(const Time& left, const Time& right);

#endif