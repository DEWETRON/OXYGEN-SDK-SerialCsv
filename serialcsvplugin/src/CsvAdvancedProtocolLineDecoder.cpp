// Copyright Heinz-Peter Liechtenecker 2020

#include "serialcsvplugin/CsvAdvancedProtocolLineDecoder.h"
#include "serialcsvplugin/regex_util.h"

namespace serialcsv {

const std::regex CsvAdvancedProtocolLineDecoder::m_header_re("(#h:)");
const std::regex CsvAdvancedProtocolLineDecoder::m_channels_re("(([a-zA-Z0-9 \\-#:\\.°\\[\\]_/]+)[,;\t]?)");
const std::regex CsvAdvancedProtocolLineDecoder::m_channel_name_re("([a-zA-Z0-9_ -]+)");
const std::regex CsvAdvancedProtocolLineDecoder::m_channel_range_re("(#r:\\s*(-?[0-9]+\\.?[0-9]*)\\s*-\\s*(-?[0-9]+\\.?[0-9]*))");
const std::regex CsvAdvancedProtocolLineDecoder::m_channel_min_re("(#min:\\s*(-?[0-9]+\\.?[0-9]*))");
const std::regex CsvAdvancedProtocolLineDecoder::m_channel_max_re("(#max:\\s*(-?[0-9]+\\.?[0-9]*))");
const std::regex CsvAdvancedProtocolLineDecoder::m_channel_unit_re("(#u:\\s*([^,;\t:#]+))");

CsvAdvancedProtocolLineDecoder::CsvAdvancedProtocolLineDecoder(const std::string &line) 
    : m_is_protocol_line(false)
    , m_is_valid(true)
    , m_line(std::move(line))
{
    if (checkForHeaderData())
    {
        // Actually a header data, do not check further
        return;
    }

    // Check any other protocol-types
}

bool CsvAdvancedProtocolLineDecoder::checkForHeaderData()
{
    std::string input(m_line);

    if (re::searchAndConsume(&input, m_header_re))
    {
        // Got a Header-Protocol
        m_is_protocol_line = true;
        m_protocol_type = serialcsv::MessageType::Header;

        // Now Interpret Channel-Header-Data
        std::string debug;

        while (re::searchAndConsume(&input, m_channels_re, &debug))
        {
            std::string channel(debug);
            ChannelDescription ch;

            // Get the mandatory Channel-Name
            if (!re::partialMatch(channel, m_channel_name_re, &ch.name))
            {
                // There was for sure an error in the protocol, return false
                m_is_valid = false;
                return true;
            }

            // Test for optional Meta-Data
            if (re::partialMatch(channel, m_channel_range_re, &ch.min, &ch.max))
            {
                ch.hasMin = true;
                ch.hasMax = true;
            }

            if (re::partialMatch(channel, m_channel_min_re, &ch.min))
            {
                ch.hasMin = true;
            }

            if (re::partialMatch(channel, m_channel_max_re, &ch.max))
            {
                ch.hasMax = true;
            }

            if (re::partialMatch(channel, m_channel_unit_re, &ch.unit))
            {
                ch.hasUnit = true;
            }

            m_channel_descriptions.push_back(std::move(ch));

            if (channel.empty())
            {
                break;
            }
        }

        return true;
    }

    return false;

}

}
