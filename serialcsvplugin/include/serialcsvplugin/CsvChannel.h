// Copyright Heinz-Peter Liechtenecker 2020
#pragma once

#include <string>
#include <mutex>
#include <vector>

#include "details/Common.h"

namespace serialcsv
{
    class CsvChannel
    {
    public:
        using Dataset = std::vector<CsvSample>;

        CsvChannel() = default;
        CsvChannel(const ChannelDescription &metadata) : m_metadata(std::move(metadata))
        {
        }

        void addSample(const CsvSample &sample)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_dataset.push_back(sample);
        }

        Dataset getAndResetDataset()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            Dataset ret{};
            ret.swap(m_dataset);

            return ret;
        }

        void setMetadata(const ChannelDescription &metadata)
        {
            m_metadata = std::move(metadata);
        }

        std::string getName() const
        {
            return m_metadata.name;
        }

        std::string getUnit() const
        {
            return m_metadata.unit;
        }

        bool hasMin() const
        {
            return m_metadata.hasMin;
        }

        bool hasMax() const
        {
            return m_metadata.hasMax;
        }

        bool hasUnit() const
        {
            return m_metadata.hasUnit;
        }

        double getMin() const
        {
            return m_metadata.min;
        }

        double getMax() const
        {
            return m_metadata.max;
        }

    private:
        ChannelDescription m_metadata;
        Dataset m_dataset;
        std::mutex m_mutex;
    };
} // namespace serialcsv
