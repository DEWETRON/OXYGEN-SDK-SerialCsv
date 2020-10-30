#include "serialcsvplugin/CsvDecoder.h"
#include "serialcsvplugin/CsvAdvancedProtocolLineDecoder.h"
#include "serialcsvplugin/CsvDataLineDecoder.h"

serialcsv::CsvDecoder::CsvDecoder()
{
}

bool serialcsv::CsvDecoder::hasTimeSource() const
{
    return !!m_currentTick;
}

void serialcsv::CsvDecoder::setTimeSource(std::function<int64_t(void)> currentTick)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_currentTick = currentTick;
}

void serialcsv::CsvDecoder::listenOnComPort(const SerialConfig &port_config)
{
    using namespace std::placeholders;
    auto cb = std::bind(&serialcsv::CsvDecoder::onLineReceived, this, _1);
    m_serial.start(port_config, cb);
}

void serialcsv::CsvDecoder::stopListening()
{
    m_serial.stop();
}

void serialcsv::CsvDecoder::clear()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_channels.clear();
}


bool serialcsv::CsvDecoder::requestHeader(const int repeat_for)
{
    using namespace std::chrono_literals;

    // Create a new promise and get the future
    m_promise_header = std::promise<void>();
    auto future_header = m_promise_header.get_future();

    // Try up to repeat_for
    for (int i = 0; i < repeat_for; i++)
    {
        m_serial.writeLn("#h");
        if (future_header.wait_for(300ms) != std::future_status::timeout)
        {
            return true;
        }
    }
    return false;
}

void serialcsv::CsvDecoder::onLineReceived(const std::string &line)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    int64_t timestamp = 0;

    if (m_currentTick)
    {
        timestamp = m_currentTick();
    }
    else
    {
        auto timestamp_t = clock::now(); // TODO: Create an Oxygen-Compatible Timestamp!
        timestamp = clock::to_time_t(timestamp_t);
    }

    // First, try to decode as a dataline
    serialcsv::CsvDataLineDecoder data(line);
    if (data.valid())
    {
        for (size_t i = m_channels.size(); i < data.size(); i++)
        {
            // Ensure all Channels do exist
            m_channels.push_back(std::make_unique<CsvChannel>());
        }

        // Add Samples to Channels
        for (int i = 0; i < data.size(); i++)
        {
            if (data.hasTimestamp())
            {
                //TODO: Use the Timestamp from the serialport
                m_channels[i]->addSample({data[i], timestamp});
            }
            else
            {
                m_channels[i]->addSample({data[i], timestamp});
            }
        }
    }
    else
    {
        serialcsv::CsvAdvancedProtocolLineDecoder proto(line);
        if (proto.isAdvancedProtocolLine())
        {
            switch (proto.getMessageType())
            {
            case serialcsv::MessageType::Header:
            {
                auto descriptions = proto.getChannelsMetadata();

                // Ensure Channels exist
                for (size_t i = m_channels.size(); i < descriptions.size(); i++)
                {
                    // Ensure all Channels do exist
                    m_channels.push_back(std::make_unique<CsvChannel>());
                }

                // Add Description to channel
                for (int i = 0; i < descriptions.size(); i++)
                {
                    m_channels[i]->setMetadata(descriptions[i]);
                }

                m_promise_header.set_value();
            }
            break;
            default:
                break;
            }
        }
    }
}
