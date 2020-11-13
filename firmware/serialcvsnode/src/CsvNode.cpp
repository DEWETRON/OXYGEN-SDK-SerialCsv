#pragma once

#include "CsvNode.h"
using namespace csvnode;

// The Actual global buffer
details::CsvString csvnode::details::buffer;

void details::CreateSampleCsv::visit(Channel &ch)
{
    etl::format_spec format;
    format.precision(details::DECIMAL_PRECISION);
    etl::to_string(ch.getValue(), details::buffer, format, true);
    details::buffer += details::SEP;
}

void details::CreateHeaderCsv::visit(Channel &ch)
{
    etl::format_spec format;
    format.precision(details::DECIMAL_PRECISION);
    details::buffer += ch.getName();
    
    if (ch.hasMinimum())
    {
        details::buffer += "#min:";
        etl::to_string(ch.getMin(), details::buffer, format, true);
    }

    if (ch.hasMaximum())
    {
        details::buffer += "#max:";
        etl::to_string(ch.getMax(), details::buffer, format, true);
    }

    if (ch.hasUnit())
    {
        details::buffer += "#u:";
        details::buffer += ch.getUnit();
    }

    details::buffer += details::SEP;
}
