#pragma once

#include <string>
#include <future>
#include <functional>
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

        void stopListening();

        void clear();

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

        bool hasTimeSource() const;
        void setTimeSource(std::function<int64_t(void)> currentTick);

    private:
        void onLineReceived(const std::string& line);
        SerialPort m_serial;
        CsvChannels m_channels;
        std::promise<void> m_promise_header;
        std::function<int64_t(void)> m_currentTick;
        std::mutex m_mutex;
    };
} // namespace serialcsv
