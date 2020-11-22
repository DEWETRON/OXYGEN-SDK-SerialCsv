#pragma once

#include "Registry.h"
using namespace csvnode;

Channel &Registry::registerChannel(const details::NameString &name, size_t *idx)
{
    Channel new_channel(name);
    if (m_idx > (details::NUM_CHANNELS - 1))
    {
        details::throwCsvNodeEx("Out of channels. Increase NUM_CHANNELS.");
    }

    if (exists(new_channel.getName()) != nullptr)
    {
        details::throwCsvNodeEx("Channel already exists.");
    }

    m_channels[m_idx] = etl::move(new_channel);

    if (idx)
    {
        *idx = m_idx;
    }

    m_idx++;

    return m_channels[m_idx - 1];
}

Channel &Registry::get(const details::NameString &name)
{
    auto ch = exists(name);

    if (ch == nullptr)
    {
        details::throwCsvNodeEx("Channel does not exist.");
    }
    return *ch;
}

void Registry::clear()
{
    for (auto &channel : m_channels)
    {
        // Overwrite with default constructor
        channel = Channel();
    }
    m_idx = 0;
}

Channel *Registry::exists(const details::NameString &name)
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
