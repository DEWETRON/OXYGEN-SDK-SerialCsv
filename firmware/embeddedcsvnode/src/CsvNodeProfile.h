#pragma once

#ifdef CSVNODE_RUNNING_ON_OS
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
// Number of Channels which can be registered
#ifdef CSVNODE_NUM_CHANNELS
        constexpr int NUM_CHANNELS = CSVNODE_NUM_CHANNELS;
#else
        constexpr int NUM_CHANNELS = 5;
#endif

// Maximum Length of the Name-String
#ifdef CSVNODE_MAX_LENGTH_NAME_STRING
        constexpr int MAX_LENGTH_NAME_STRING = CSVNODE_MAX_LENGTH_NAME_STRING;
#else
        constexpr int MAX_LENGTH_NAME_STRING = 20;
#endif

// Maximum Length for Short Strings (e.g. Unit)
#ifdef CSVNODE_MAX_LENGTH_SHORT_STRING
        constexpr int MAX_LENGTH_SHORT_STRING = CSVNODE_MAX_LENGTH_SHORT_STRING;
#else
        constexpr int MAX_LENGTH_SHORT_STRING = 5;
#endif

// Maximum Length of CSV-Strings
#ifdef CSVNODE_MAX_LENGTH_CSV_STRING
        constexpr int MAX_LENGTH_CSV_STRING = CSVNODE_MAX_LENGTH_CSV_STRING;
#else
        constexpr int MAX_LENGTH_CSV_STRING = MAX_LENGTH_NAME_STRING * NUM_CHANNELS;
#endif

// The Decimal-Precision when Formatting
#ifdef CSVNODE_DECIMAL_PRECISION
        constexpr int DECIMAL_PRECISION = CSVNODE_DECIMAL_PRECISION;
#else
        constexpr int DECIMAL_PRECISION = 2;
#endif

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

        // Maximum Length of incoming protocol-strings
        constexpr int MAX_LENGTH_INCOMING_STRING = 10;

        // This function must either throw an exception or interrupt execution of the program
        static inline void throwCsvNodeEx(const char *msg)
        {
#ifdef CSVNODE_RUNNING_ON_OS
            throw std::runtime_error(msg);
#endif
        }

        // Typedefs
        using NameString = etl::string<MAX_LENGTH_NAME_STRING>;
        using CsvString = etl::string<MAX_LENGTH_CSV_STRING>;
        using ShortString = etl::string<MAX_LENGTH_SHORT_STRING>;
        using IncomingString = etl::string<MAX_LENGTH_INCOMING_STRING>;
    } // namespace details
} // namespace csvnode
