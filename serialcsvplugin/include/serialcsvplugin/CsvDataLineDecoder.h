#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace re2
{
    class RE2;
} // namespace re2

namespace serialcsv
{
    class CsvDataLineDecoder
    {
    public:
        CsvDataLineDecoder(const std::string &line);
        bool at(const std::size_t i, double &v) const;
        std::size_t size() const
        {
            return m_data.size();
        }

        bool valid() const
        {
            return m_valid;
        }

        bool hasTimestamp() const
        {
            return m_has_timestamp;
        }

        unsigned long getTimestamp() const
        {
            return m_timestamp;
        }

        double &operator[](size_t idx)
        {
            if (idx >= m_data.size())
            {
                throw std::out_of_range("Index out of range. There where less channels in this message.");
            }

            return m_data[idx];
        }

    private:
        std::string m_line;
        std::vector<double> m_data;
        unsigned long m_timestamp;
        bool m_valid;
        bool m_has_timestamp;
        const static re2::RE2 m_value_re;
        const static re2::RE2 m_timestamp_re;
        const static re2::RE2 m_invalid_re;
    };
} // namespace serialcsv
