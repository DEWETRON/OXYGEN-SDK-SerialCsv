// Copyright Heinz-Peter Liechtenecker 2020

#include "serialcsvplugin/CsvDataLineDecoder.h"
#include "serialcsvplugin/regex_util.h"
#include <iostream>

namespace serialcsv {

const std::regex CsvDataLineDecoder::m_value_re(R"(([+-]?[0-9]+\.?[0-9]*)[,;\t]?\s?)");
const std::regex CsvDataLineDecoder::m_timestamp_re("(#t:([0-9]*)[,;\t]?)");
const std::regex CsvDataLineDecoder::m_invalid_re("[^\\.,;\t\\s\\d-]");

CsvDataLineDecoder::CsvDataLineDecoder(const std::string &line)
    : m_line(std::move(line)),
      m_data{},
      m_timestamp(0),
      m_valid(true),
      m_has_timestamp(false)
{
    auto input = line;

    // Consume Timestamp
    m_has_timestamp = re::searchAndConsume(&input, m_timestamp_re, &m_timestamp);

    // Check if the string is valid
    {
        if (std::regex_search(input, m_invalid_re))
        {
            m_valid = false;
            return;
        }
    }

    // Consume Values
    double value = 0.0;

    while (re::searchAndConsume(&input, m_value_re, &value))
    {
        m_data.emplace_back(value);
    }
}

bool CsvDataLineDecoder::at(const std::size_t i, double &v) const
{
    if (size() > i)
    {
        v = m_data[i];
        return true;
    }
    else
    {
        return false;
    }
}

}
