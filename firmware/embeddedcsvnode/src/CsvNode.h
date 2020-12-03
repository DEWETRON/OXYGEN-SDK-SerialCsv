#pragma once

#include "CsvNodeProfile.h"

#include "etl/to_string.h"
#include "etl/format_spec.h"
#include "etl/visitor.h"

#include "details/Registry.h"
#include "details/Visitors.h"
#include "Channel.h"

#ifdef ARDUINO
#include "Arduino.h"
#endif

namespace csvnode
{
    template <typename Clock,
              typename Serial,
              size_t NUM_CHANNELS = 5,
              uint32_t DECIMAL_PRECISION = 2,
              size_t OUTGOING_BUFFER_SIZE = 100,
              size_t INCOMING_BUFFER_SIZE = 10>
    class SerialCsvNode
    {
    public:
        SerialCsvNode();
        void process();
        void enablePrefixTimestamp()
        {
            m_prefix_timestamp = true;
        }
        etl::string<OUTGOING_BUFFER_SIZE> &getCurrentBuffer() const
        {
            return m_buffer;
        }
        Clock &getClock()
        {
            return m_clock;
        }
        Serial &getSerial()
        {
            return m_serial;
        }
        details::Registry<Serial, NUM_CHANNELS> &getRegistry()
        {
            return m_registry;
        }
        Channel &registerChannel(const etl::string_view &name, size_t *idx = nullptr)
        {
            return m_registry.registerChannel(name, idx);
        }
        Channel &getChannel(const etl::string_view &name)
        {
            return m_registry.get(name);
        }
        Channel &getChannel(const size_t idx)
        {
            return m_registry[idx];
        }
        Channel &operator[](size_t idx)
        {
            return m_registry[idx];
        }
        size_t getSize() const
        {
            return m_registry.getSize();
        }

        void begin(const etl::string_view &name, const unsigned long baudrate, const unsigned int Ts)
        {
            m_name = name;
            m_sampling_interval = Ts;
            m_begin_called = true;
            m_sample_visitor.setBuffer(&m_buffer);
            m_header_visitor.setBuffer(&m_buffer);
            m_serial.begin(baudrate);
        }
        void run();

    private:
        void sample();
        void sendHeader();
        etl::string_view m_name;
        bool m_prefix_timestamp;
        bool m_begin_called;

        Serial m_serial;
        Clock m_clock;
        typename Clock::milliseconds m_sampling_interval;
        typename Clock::milliseconds m_last_sampling_timepoint;

        // The Channel-Registry
        details::Registry<Serial, NUM_CHANNELS> m_registry;

        // The Visitors for creating CSV-Strings
        details::CreateSampleCsv<OUTGOING_BUFFER_SIZE, DECIMAL_PRECISION> m_sample_visitor;
        details::CreateHeaderCsv<OUTGOING_BUFFER_SIZE, DECIMAL_PRECISION> m_header_visitor;

        // Buffer in RAM for incoming Strings
        etl::string<INCOMING_BUFFER_SIZE> m_read_buffer;

        // Buffer in RAM for outgoing Strings
        etl::string<OUTGOING_BUFFER_SIZE> m_buffer;
    };

    template <typename Clock,
              typename Serial,
              size_t NUM_CHANNELS,
              uint32_t DECIMAL_PRECISION,
              size_t OUTGOING_BUFFER_SIZE,
              size_t INCOMING_BUFFER_SIZE>
    SerialCsvNode<Clock,
                  Serial,
                  NUM_CHANNELS,
                  DECIMAL_PRECISION,
                  OUTGOING_BUFFER_SIZE,
                  INCOMING_BUFFER_SIZE>::SerialCsvNode()
        : m_sampling_interval(0),
          m_last_sampling_timepoint(0),
          m_prefix_timestamp(false),
          m_begin_called(false)
    {
    }

    template <typename Clock,
              typename Serial,
              size_t NUM_CHANNELS,
              uint32_t DECIMAL_PRECISION,
              size_t OUTGOING_BUFFER_SIZE,
              size_t INCOMING_BUFFER_SIZE>
    void SerialCsvNode<Clock,
                       Serial,
                       NUM_CHANNELS,
                       DECIMAL_PRECISION,
                       OUTGOING_BUFFER_SIZE,
                       INCOMING_BUFFER_SIZE>::sample()
    {
        m_buffer.clear();

        if (m_prefix_timestamp)
        {
            m_buffer += "#t:";
            etl::to_string(m_clock.now(), m_buffer, true);
            m_buffer += details::SEP;
        }

        for (auto &&channel : m_registry)
        {
            channel.accept(m_sample_visitor);
        }

        m_buffer += details::EOL;

        if (m_buffer.is_truncated())
        {
            details::throwCsvNodeEx<Serial>("The CSV-Node Buffer is truncated. Increase size!");
        }

        m_serial.write(m_buffer.c_str(), m_buffer.size());
    }

    template <typename Clock,
              typename Serial,
              size_t NUM_CHANNELS,
              uint32_t DECIMAL_PRECISION,
              size_t OUTGOING_BUFFER_SIZE,
              size_t INCOMING_BUFFER_SIZE>
    void SerialCsvNode<Clock,
                       Serial,
                       NUM_CHANNELS,
                       DECIMAL_PRECISION,
                       OUTGOING_BUFFER_SIZE,
                       INCOMING_BUFFER_SIZE>::sendHeader()
    {
        m_buffer.clear();
        m_buffer += "#h:";
        for (auto &&channel : m_registry)
        {
            channel.accept(m_header_visitor);
        }

        m_buffer += details::EOL;

        if (m_buffer.is_truncated())
        {
            details::throwCsvNodeEx<Serial>("The CSV-Node Buffer is truncated. Increase size!");
        }

        m_serial.write(m_buffer.c_str(), m_buffer.size());
    }

    template <typename Clock,
              typename Serial,
              size_t NUM_CHANNELS,
              uint32_t DECIMAL_PRECISION,
              size_t OUTGOING_BUFFER_SIZE,
              size_t INCOMING_BUFFER_SIZE>
    void SerialCsvNode<Clock,
                       Serial,
                       NUM_CHANNELS,
                       DECIMAL_PRECISION,
                       OUTGOING_BUFFER_SIZE,
                       INCOMING_BUFFER_SIZE>::run()
    {
        static bool first_run = true;

        if (!m_begin_called)
        {
            return;
        }

        if (first_run)
        {
            // Start Sampling from timepoint now
            m_last_sampling_timepoint = m_clock.now();
            first_run = false;
        }

        while (m_serial.available())
        {
            m_read_buffer += static_cast<char>(m_serial.read());
            if (m_read_buffer.find("\n") != etl::string<INCOMING_BUFFER_SIZE>::npos)
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
        // Use SERIAL_PORT_MONITOR define from Arduino Core for the default Arduino Implementation
        void write(const char *c, size_t len)
        {
            SERIAL_PORT_MONITOR.write(c, len);
        }

        int read()
        {
            return SERIAL_PORT_MONITOR.read();
        }

        unsigned int available()
        {
            return SERIAL_PORT_MONITOR.available();
        }

        void begin(const unsigned long baudrate)
        {
            SERIAL_PORT_MONITOR.begin(baudrate);
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
    using DefaultArduinoCsvNode = SerialCsvNode<ArduinoClock, ArduinoSerial>;
#endif
} // namespace csvnode

#ifdef ARDUINO
// The one and only Default-Serial-Node Instance (which only gets linked on usage)
extern csvnode::DefaultArduinoCsvNode CsvNode;
#endif
