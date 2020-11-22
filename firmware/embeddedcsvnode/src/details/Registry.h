#pragma once

#include "../CsvNodeProfile.h"
#include "../Channel.h"

#include "etl/array.h"

namespace csvnode
{
    class Registry
    {
        // Make the Iterator a Friend so that it can access the array
        template <typename T>
        friend class RegistryIteratorType;

    public:
        using ChannelArray = etl::array<Channel, details::NUM_CHANNELS>;
        static Registry &instance()
        {
            static Registry reg;
            return reg;
        }
        Channel &operator[](size_t idx)
        {
            if (idx >= details::NUM_CHANNELS)
            {
                details::throwCsvNodeEx("Out of rane.");
            }

            return m_channels[idx];
        }
        Channel &registerChannel(const details::NameString &name, size_t *idx = nullptr);
        Channel &get(const details::NameString &name);
        void clear();
        size_t getSize() const
        {
            return m_idx; //The actual size is always the current idx
        }

    private:
        Registry() : m_idx(0){};
        Registry(const Registry &) = delete;
        const Registry &operator=(const Registry &) = delete;

        Channel *exists(const details::NameString &name);

        ChannelArray m_channels = {};
        size_t m_idx;
    };

    template <typename T>
    class RegistryIteratorType
    {
    public:
        RegistryIteratorType(T &collection, size_t const index) : index(index), collection(collection)
        {
        }

        bool operator!=(RegistryIteratorType const &other) const
        {
            return index != other.index;
        }

        Channel &operator*() const
        {
            return collection.m_channels[index];
        }

        RegistryIteratorType const &operator++()
        {
            ++index;
            return *this;
        }

    private:
        size_t index;
        T &collection;
    };

    using RegistryIterator = RegistryIteratorType<Registry>;

    inline RegistryIterator begin(Registry &collection)
    {
        return RegistryIterator(collection, 0);
    }

    inline RegistryIterator end(Registry &collection)
    {
        return RegistryIterator(collection, collection.getSize());
    }

    inline Channel &registerChannel(const details::NameString &name, size_t *idx = nullptr)
    {
        return Registry::instance().registerChannel(name, idx);
    }

    inline Channel &getChannel(const details::NameString &name)
    {
        return Registry::instance().get(name);
    }

    inline Channel &getChannel(const size_t idx)
    {
        return Registry::instance()[idx];
    }
} // namespace csvnode
