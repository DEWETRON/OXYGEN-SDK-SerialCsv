#pragma once

#ifdef CSVNODE_THROW_EXCEPTION
#include <stdexcept>
#endif

#include <etl_profile.h>
#include "etl/string.h"
#include "etl/array.h"
#include "etl/string.h"

namespace csvnode
{
    namespace details
    {
// The End-of-Line Character
#ifdef CSVNODE_EOL
        constexpr char EOL[2] = CSVNODE_EOL;
#else
        constexpr char EOL[2] = "\n";
#endif

// The Seperator
#ifdef CSVNODE_SEP
        constexpr char SEP[2] = CSVNODE_SEP;
#else
        constexpr char SEP[2] = ",";
#endif
        class SerialPort
        {
        public:
            virtual void write(const etl::string_view msg) = 0;
        };

        static inline void throwCsvNodeEx(SerialPort& port, const etl::string_view msg)
        {
            port.write(msg);

#ifdef CSVNODE_THROW_EXCEPTION
            throw std::runtime_error(msg.data());
#endif
            while (true)
                ;
        }
    } // namespace details
} // namespace csvnode
