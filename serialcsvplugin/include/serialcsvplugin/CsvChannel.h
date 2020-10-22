#pragma once

#include <string>
#include <queue>
#include <mutex>

#include "details/Common.h"

namespace serialcsv
{
    class CsvChannel
    {
    public:
        using Dataset = std::queue<CsvSample>;

        CsvChannel() = default;
        CsvChannel(const ChannelDescription &metadata) : m_metadata(std::move(metadata))
        {
        }

        void addSample(const CsvSample &sample)
        {
            std::unique_lock<std::mutex> lock;
            m_dataset.push(sample);
        }

        Dataset getAndResetDataset()
        {
            std::unique_lock<std::mutex> lock;

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
