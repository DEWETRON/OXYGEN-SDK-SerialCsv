#pragma once

#include <string>
#include <future>
#include "details/Common.h"
#include "details/SerialPort.h"
#include "CsvChannel.h"

namespace serialcsv
{
    class CsvDecoder
    {
    public:
        using CsvChannels = std::vector<std::unique_ptr<CsvChannel>>;
        CsvDecoder();
        void listenOnComPort(const SerialConfig &port_config);
        bool requestHeader(const int repeat_for = 5);

        CsvChannels& getChannels()
        {
            return m_channels;
        }

        SerialPort& getSerial()
        {
            return m_serial;
        }

        size_t numberOfChannels() const 
        {
            return m_channels.size();
        }

    private:
        void onLineReceived(const std::string& line);
        SerialPort m_serial;
        CsvChannels m_channels;
        std::promise<void> m_promise_header;
    };
} // namespace serialcsv
