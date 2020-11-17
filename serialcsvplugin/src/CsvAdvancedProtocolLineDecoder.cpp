// Copyright Heinz-Peter Liechtenecker 2020

#include <re2/re2.h>
#include "serialcsvplugin/CsvAdvancedProtocolLineDecoder.h"

const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_header_re("(#h:)");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channels_re("(([a-zA-Z0-9 \\-#:\\.°\\[\\]]+)[,;\t]?)");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channel_name_re("([a-zA-Z0-9 -]+)");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channel_range_re("(#r:\\s*(-?[0-9]+\\.?[0-9]*)\\s*-\\s*(-?[0-9]+\\.?[0-9]*))");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channel_min_re("(#min:\\s*(-?[0-9]+\\.?[0-9]*))");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channel_max_re("(#max:\\s*(-?[0-9]+\\.?[0-9]*))");
const re2::RE2 serialcsv::CsvAdvancedProtocolLineDecoder::m_channel_unit_re("(#u:\\s*([^,;\t:#]+))");

serialcsv::CsvAdvancedProtocolLineDecoder::CsvAdvancedProtocolLineDecoder(const std::string &line) : m_line(std::move(line)),
                                                                                                     m_is_protocol_line(false),
                                                                                                     m_is_valid(true)
{
    if (checkForHeaderData())
    {
        // Actually a header data, do not check further
        return;
    }

    // Check any other protocol-types
}

bool serialcsv::CsvAdvancedProtocolLineDecoder::checkForHeaderData()
{
    using namespace re2;
    StringPiece input(m_line);

    if (RE2::FindAndConsume(&input, m_header_re))
    {
        // Got a Header-Protocol
        m_is_protocol_line = true;
        m_protocol_type = serialcsv::MessageType::Header;

        // Now Interpret Channel-Header-Data
        std::string debug;

        while (RE2::FindAndConsume(&input, m_channels_re, nullptr, &debug))
        {
            StringPiece channel(debug);
            ChannelDescription ch;

            // Get the mandatory Channel-Name
            if (!RE2::PartialMatch(channel, m_channel_name_re, &ch.name))
            {
                // There was for sure an error in the protocol, return false
                m_is_valid = false;
                return true;
            }

            // Test for optional Meta-Data
            if (RE2::PartialMatch(channel, m_channel_range_re, nullptr, &ch.min, &ch.max))
            {
                ch.hasMin = true;
                ch.hasMax = true;
            }

            if (RE2::PartialMatch(channel, m_channel_min_re, nullptr, &ch.min))
            {
                ch.hasMin = true;
            }

            if (RE2::PartialMatch(channel, m_channel_max_re, nullptr, &ch.max))
            {
                ch.hasMax = true;
            }

            if (RE2::PartialMatch(channel, m_channel_unit_re, nullptr, &ch.unit))
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
    else
    {
        return false;
    }
}

