#pragma once

#include <chrono>
#include <string>

namespace serialcsv
{
    using clock = std::chrono::system_clock;
    using time_point = clock::time_point;

    struct CsvSample
    {
        CsvSample(double v, int64_t t)
        {
            value = v;
            time = t;
        }
        
        int64_t time;
        double value;
    };

    struct ChannelDescription
    {
        ChannelDescription()
        {
            unit = "";
            name = "";
            min = 0.0;
            max = 0.0;
            hasMin = false;
            hasMax = false;
            hasUnit = false;
        }

        double min;
        double max;
        bool hasMin;
        bool hasMax;
        bool hasUnit;
        std::string unit;
        std::string name;
    };

    enum class MessageType
    {
        Header,
    };
}
