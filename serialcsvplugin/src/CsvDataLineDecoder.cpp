// Copyright Heinz-Peter Liechtenecker 2020

#include <re2/re2.h>
#include "serialcsvplugin/CsvDataLineDecoder.h"

const re2::RE2 serialcsv::CsvDataLineDecoder::m_value_re("(?P<valuematch>(?P<value>-?[0-9]+\\.?[0-9]*)[,;\t]?\\s?)");
const re2::RE2 serialcsv::CsvDataLineDecoder::m_timestamp_re("(?P<timematch>#t:(?P<time>[0-9]*)[,;\t]?)");
const re2::RE2 serialcsv::CsvDataLineDecoder::m_invalid_re("[^\\.,;\t\\s\\d-]");

serialcsv::CsvDataLineDecoder::CsvDataLineDecoder(const std::string &line) : m_line(std::move(line)),
                                                      m_data{},
                                                      m_valid(true),
                                                      m_timestamp(0),
                                                      m_has_timestamp(false)
{
    using namespace re2;
    StringPiece input(m_line);

    // Consume Timestamp
    m_has_timestamp = RE2::FindAndConsume(&input, m_timestamp_re, nullptr, &m_timestamp);

    // Check if the string is valid
    if (RE2::FindAndConsume(&input, m_invalid_re))
    {
        m_valid = false;
        return;
    }

    // Consume Values
    double value = 0.0;
    while (RE2::FindAndConsume(&input, m_value_re, nullptr, &value))
    {
        m_data.emplace_back(value);
    }
}

bool serialcsv::CsvDataLineDecoder::at(const std::size_t i, double &v) const
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
