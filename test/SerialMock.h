#pragma once

#include <string>

inline std::string written;
inline std::string to_read;

struct MockSerial
{
    void write(const char *c, size_t length)
    {
        written = std::string(c, length);
    }

    int read()
    {
        if (to_read.empty())
        {
            return -1;
        }
        else
        {
            char ret = to_read.at(0);
            to_read.erase(0, 1);
            return ret;
        }
    }

    size_t available()
    {
        return to_read.size();
    }

    void begin(const unsigned long)
    {
    }
};

struct MockClock
{
    using milliseconds = long long;
    MockClock()
    {
        offset = 0;
        tick = 0;
    }

    milliseconds now()
    {
        return tick - offset;
    }

    void reset()
    {
        offset = tick;
    }

    milliseconds offset;
    milliseconds tick;
};
