#pragma once

#include "etl/visitor.h"

namespace csvnode
{
    class Channel;
    class ChannelVisitor : public etl::visitor<Channel>
    {
    };

    class ChannelBase : public etl::visitable<ChannelVisitor>
    {
    };

    class Channel : public ChannelBase
    {
    public:
        Channel()
        {
            m_value = 0;
            m_min = 0;
            m_max = 0;
            m_has_minimum = false;
            m_has_maximum = false;
            m_has_unit = false;
        }
        Channel(const etl::string_view& name) : m_name(name)
        {
            m_value = 0;
            m_min = 0;
            m_max = 0;
            m_has_minimum = false;
            m_has_maximum = false;
            m_has_unit = false;
        }
        const etl::string_view &getName() const
        {
            return m_name;
        }
        Channel &logValue(const double v)
        {
            m_value = v;
            return *this;
        }
        double getValue() const
        {
            return m_value;
        }
        Channel &setUnit(const etl::string_view& u)
        {
            m_unit = u;
            m_has_unit = true;
            return *this;
        }
        const etl::string_view &getUnit()
        {
            return m_unit;
        }
        bool hasUnit()
        {
            return m_has_unit;
        }
        Channel &setMax(double max)
        {
            m_max = max;
            m_has_maximum = true;
            return *this;
        }
        double getMax()
        {
            return m_max;
        }
        bool hasMaximum()
        {
            return m_has_maximum;
        }
        Channel &setMin(double min)
        {
            m_min = min;
            m_has_minimum = true;
            return *this;
        }
        double getMin()
        {
            return m_min;
        }
        bool hasMinimum()
        {
            return m_has_minimum;
        }
        void accept(ChannelVisitor &visitor)
        {
            visitor.visit(*this);
        }

    private:
        etl::string_view m_name;
        etl::string_view m_unit;
        double m_min;
        double m_max;
        double m_value;
        bool m_has_minimum;
        bool m_has_maximum;
        bool m_has_unit;
    };
} // namespace csvnode
