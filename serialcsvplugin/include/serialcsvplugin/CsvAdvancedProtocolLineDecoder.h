// Copyright Heinz-Peter Liechtenecker 2020
#pragma once

#include <string>
#include <vector>

#include "details/Common.h"

namespace re2
{
    class RE2;
} // namespace re2

namespace serialcsv
{
    class CsvAdvancedProtocolLineDecoder
    {
    public:
        using ChannelDescriptions = std::vector<ChannelDescription>;

        CsvAdvancedProtocolLineDecoder(const std::string &line);
        bool isAdvancedProtocolLine() const {
            return m_is_protocol_line;
        }
        MessageType getMessageType() const {
            return m_protocol_type;
        }
        ChannelDescriptions getChannelsMetadata() const {
            return m_channel_descriptions;
        }
        

    private:
        bool checkForHeaderData();

        bool m_is_protocol_line;
        bool m_is_valid;
        std::string m_line;
        MessageType m_protocol_type;
        ChannelDescriptions m_channel_descriptions;

        const static re2::RE2 m_header_re;
        const static re2::RE2 m_channels_re;
        const static re2::RE2 m_channel_name_re;
        const static re2::RE2 m_channel_range_re;
        const static re2::RE2 m_channel_min_re;
        const static re2::RE2 m_channel_max_re;
        const static re2::RE2 m_channel_unit_re;
    };
} // namespace serialcsv
