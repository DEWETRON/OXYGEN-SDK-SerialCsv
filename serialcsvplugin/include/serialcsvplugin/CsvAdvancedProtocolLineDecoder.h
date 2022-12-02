// Copyright Heinz-Peter Liechtenecker 2020
#pragma once

#include <string>
#include <regex>
#include <vector>

#include "details/Common.h"

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
        size_t getNumberOfChannels() const {
            return m_channel_descriptions.size();
        }
        
    private:
        bool checkForHeaderData();

        bool m_is_protocol_line;
        bool m_is_valid;
        std::string m_line;
        MessageType m_protocol_type;
        ChannelDescriptions m_channel_descriptions;

        const static std::regex m_header_re;
        const static std::regex m_channels_re;
        const static std::regex m_channel_name_re;
        const static std::regex m_channel_range_re;
        const static std::regex m_channel_min_re;
        const static std::regex m_channel_max_re;
        const static std::regex m_channel_unit_re;
    };
} // namespace serialcsv
