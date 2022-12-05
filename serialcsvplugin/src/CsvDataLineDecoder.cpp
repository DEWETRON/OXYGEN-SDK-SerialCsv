// Copyright Heinz-Peter Liechtenecker 2020

#include "serialcsvplugin/CsvDataLineDecoder.h"

namespace serialcsv {

#if 0
const std::regex CsvDataLineDecoder::m_value_re("(?P<valuematch>(?P<value>[+-]?[0-9]+\\.?[0-9]*)[,;\t]?\\s?)");
const std::regex CsvDataLineDecoder::m_timestamp_re("(?P<timematch>#t:(?P<time>[0-9]*)[,;\t]?)");
const std::regex CsvDataLineDecoder::m_invalid_re("[^\\.,;\t\\s\\d-]");
#endif

CsvDataLineDecoder::CsvDataLineDecoder(const std::string &line)
    : m_line(std::move(line)),
      m_data{},
      m_timestamp(0),
      m_valid(true),
      m_has_timestamp(false)
{
    // using namespace re2;
    // StringPiece input(m_line);

    // // Consume Timestamp
    // m_has_timestamp = RE2::FindAndConsume(&input, m_timestamp_re, nullptr, &m_timestamp);

    // // Check if the string is valid
    // if (RE2::FindAndConsume(&input, m_invalid_re))
    // {
    //     m_valid = false;
    //     return;
    // }

    // // Consume Values
    // double value = 0.0;
    // while (RE2::FindAndConsume(&input, m_value_re, nullptr, &value))
    // {
    //     m_data.emplace_back(value);
    // }
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
