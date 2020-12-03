#pragma once

#include "etl/array.h"
#include "../Channel.h"

namespace csvnode
{
    namespace details
    {

        template <typename Serial, size_t NUM_CHANNELS>
        class Registry
        {
            // Make the Iterator a Friend so that it can access the array
            template <typename T>
            friend class RegistryIteratorType;

        public:
            using ChannelArray = etl::array<Channel, NUM_CHANNELS>;
            Registry() : m_idx(0){};
            Registry(const Registry &) = delete;
            const Registry &operator=(const Registry &) = delete;

            Channel &operator[](size_t idx)
            {
                if (idx >= NUM_CHANNELS)
                {
                    details::throwCsvNodeEx<Serial>("Out of range.");
                }

                return m_channels[idx];
            }

            Channel &registerChannel(const etl::string_view &name, size_t *idx = nullptr)
            {
                Channel new_channel(name);
                if (m_idx > (NUM_CHANNELS - 1))
                {
                    details::throwCsvNodeEx<Serial>("Out of channels. Increase NUM_CHANNELS.");
                }

                if (exists(new_channel.getName()) != nullptr)
                {
                    details::throwCsvNodeEx<Serial>("Channel already exists.");
                }

                m_channels[m_idx] = etl::move(new_channel);

                if (idx)
                {
                    *idx = m_idx;
                }

                m_idx++;

                return m_channels[m_idx - 1];
            }

            Channel &get(const etl::string_view &name)
            {
                auto ch = exists(name);

                if (ch == nullptr)
                {
                    details::throwCsvNodeEx<Serial>("Channel does not exist.");
                }
                return *ch;
            }

            void clear()
            {
                for (auto &channel : m_channels)
                {
                    // Overwrite with default constructor
                    channel = Channel();
                }
                m_idx = 0;
            }

            size_t getSize() const
            {
                return m_idx; //The actual size is always the current idx
            }

        private:
            Channel *exists(const etl::string_view &name)
            {
                auto it = etl::find_if(m_channels.begin(), m_channels.end(), [&](Channel &channel) {
                    return channel.getName() == name;
                });

                if (it != m_channels.end())
                {
                    return it;
                }
                else
                {
                    return nullptr;
                }
            }

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

        template <typename Serial, size_t NUM_CHANNELS>
        using RegistryIterator = RegistryIteratorType<Registry<Serial, NUM_CHANNELS>>;

        template <typename Serial, size_t NUM_CHANNELS>
        inline RegistryIterator<Serial, NUM_CHANNELS> begin(Registry<Serial, NUM_CHANNELS> &collection)
        {
            return RegistryIterator<Serial, NUM_CHANNELS>(collection, 0);
        }

        template <typename Serial, size_t NUM_CHANNELS>
        inline RegistryIterator<Serial, NUM_CHANNELS> end(Registry<Serial, NUM_CHANNELS> &collection)
        {
            return RegistryIterator<Serial, NUM_CHANNELS>(collection, collection.getSize());
        }
    } // namespace details
} // namespace csvnode
