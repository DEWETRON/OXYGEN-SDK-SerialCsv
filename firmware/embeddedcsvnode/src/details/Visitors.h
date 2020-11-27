#pragma once

#include "etl/to_string.h"
#include "etl/format_spec.h"
#include "etl/visitor.h"

#include "../Channel.h"

namespace csvnode
{
    namespace details
    {
        template<size_t OUTGOING_BUFFER_SIZE>
        class VisitorBuffer
        {
        public:
            void setBuffer(etl::string<OUTGOING_BUFFER_SIZE> *the_buffer)
            {
                m_buffer = the_buffer;
            }
        protected:
            etl::string<OUTGOING_BUFFER_SIZE> *getBuffer()
            {
                return m_buffer;
            }

            etl::string<OUTGOING_BUFFER_SIZE> *m_buffer = nullptr;
        };

        template <size_t OUTGOING_BUFFER_SIZE, uint32_t DECIMAL_PRECISION>
        class CreateSampleCsv : public ChannelVisitor, public VisitorBuffer<OUTGOING_BUFFER_SIZE>
        {
        public:
            void visit(Channel &ch)
            {
                if (!getBuffer())
                    return;
                etl::string<OUTGOING_BUFFER_SIZE> &buffer = *getBuffer();

                etl::format_spec format;
                format.precision(DECIMAL_PRECISION);
                etl::to_string(ch.getValue(), buffer, format, true);
                buffer += details::SEP;
            }
        };

        template <size_t OUTGOING_BUFFER_SIZE, uint32_t DECIMAL_PRECISION>
        class CreateHeaderCsv : public ChannelVisitor, public VisitorBuffer<OUTGOING_BUFFER_SIZE>
        {
        public:
            void visit(Channel &ch)
            {
                if (!getBuffer())
                    return;
                
                etl::string<OUTGOING_BUFFER_SIZE> &buffer = *getBuffer();
                etl::format_spec format;
                format.precision(DECIMAL_PRECISION);
                const etl::string_view &name = ch.getName();
                buffer.append(name.begin(), name.end());

                if (ch.hasMinimum())
                {
                    buffer += "#min:";
                    etl::to_string(ch.getMin(), buffer, format, true);
                }

                if (ch.hasMaximum())
                {
                    buffer += "#max:";
                    etl::to_string(ch.getMax(), buffer, format, true);
                }

                if (ch.hasUnit())
                {
                    buffer += "#u:";
                    const etl::string_view &unit = ch.getUnit();
                    buffer.append(unit.begin(), unit.end());
                }

                buffer += details::SEP;
            }
        };
    } // namespace details
} // namespace csvnode
