#pragma once

#include "CsvNodeProfile.h"

#include "etl/to_string.h"
#include "etl/format_spec.h"
#include "etl/visitor.h"

#include "details/Registry.h"
#include "Channel.h"

#ifdef ARDUINO
#include "Arduino.h"
#endif

namespace csvnode
{
    namespace details
    {
        // The buffer shared between the Node and the Visitors
        extern CsvString buffer;
        class CreateSampleCsv : public ChannelVisitor
        {
        public:
            void visit(Channel &ch);
        };

        class CreateHeaderCsv : public ChannelVisitor
        {
        public:
            void visit(Channel &ch);
        };
    } // namespace details

    template <typename Clock, typename Serial>
    class SerialCsvNode
    {
    public:
        SerialCsvNode(const details::NameString name, const unsigned int Ts);
        void process();
        void enablePrefixTimestamp()
        {
            m_prefix_timestamp = true;
        }
        details::CsvString &getCurrentBuffer() const
        {
            return details::buffer;
        }
        Clock &getClock()
        {
            return m_clock;
        }
        Serial &getSerial()
        {
            return m_serial;
        }
        void run();

    private:
        void sample();
        void sendHeader();

        Serial m_serial;
        Clock m_clock;
        typename Clock::milliseconds m_sampling_interval;
        typename Clock::milliseconds m_last_sampling_timepoint;
        bool m_prefix_timestamp;
        details::NameString m_name;
        details::CreateSampleCsv m_sample_visitor;
        details::CreateHeaderCsv m_header_visitor;
        details::IncomingString m_read_buffer;
    };

    template <typename Clock, typename Serial>
    SerialCsvNode<Clock, Serial>::SerialCsvNode(const details::NameString name,
                                                const unsigned int sampling_interval)
        : m_sampling_interval(sampling_interval),
          m_last_sampling_timepoint(0),
          m_prefix_timestamp(false),
          m_name(move(name))

    {
    }

    template <typename Clock, typename Serial>
    void SerialCsvNode<Clock, Serial>::sample()
    {
        details::buffer.clear();

        if (m_prefix_timestamp)
        {
            details::buffer += "#t:";
            etl::to_string(m_clock.now(), details::buffer, true);
            details::buffer += details::SEP;
        }

        for (auto &&channel : Registry::instance())
        {
            channel.accept(m_sample_visitor);
        }

        details::buffer += details::EOL;
        m_serial.write(details::buffer.c_str(), details::buffer.size());
    }

    template <typename Clock, typename Serial>
    void SerialCsvNode<Clock, Serial>::sendHeader()
    {
        details::buffer.clear();
        details::buffer += "#h:";
        for (auto &&channel : Registry::instance())
        {
            channel.accept(m_header_visitor);
        }

        details::buffer += details::EOL;
        m_serial.write(details::buffer.c_str(), details::buffer.size());
    }

    template <typename Clock, typename Serial>
    void SerialCsvNode<Clock, Serial>::run()
    {
        static bool first_run = true;

        if (first_run)
        {
            // Start Sampling from timepoint now
            m_last_sampling_timepoint = m_clock.now();
            first_run = false;
        }

        while (m_serial.available())
        {
            m_read_buffer += static_cast<char>(m_serial.read());
            if (m_read_buffer.find("\n") != details::IncomingString::npos)
            {
                if (m_read_buffer == "#h\n")
                {
                    // Plugin requested header data
                    sendHeader();
                    m_read_buffer.clear();
                }
                if (m_read_buffer == "#t0\n")
                {
                    // Plugin reqeusted a timestamp-reset
                    m_clock.reset();
                    m_last_sampling_timepoint = m_clock.now();
                }

                m_read_buffer.clear();
            }
        }

        // Sample
        if ((m_clock.now() - m_last_sampling_timepoint) >= m_sampling_interval)
        {
            m_last_sampling_timepoint = m_clock.now();
            sample();
        }
    }

#ifdef ARDUINO
    struct ArduinoSerial
    {
        void write(const char *c, size_t len)
        {
            Serial.write(c, len);
        }

        int read()
        {
            return Serial.read();
        }

        unsigned int available()
        {
            return Serial.available();
        }
    };

    struct ArduinoClock
    {
        using milliseconds = unsigned long;
        ArduinoClock()
        {
            offset = millis();
        }

        milliseconds now()
        {
            return millis() - offset;
        }

        void reset()
        {
            offset = millis();
        }

        milliseconds offset;
    };
    using ArduinoCsvNode = SerialCsvNode<ArduinoClock, ArduinoSerial>;
#endif

} // namespace csvnode
