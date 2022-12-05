#pragma once

#include <regex>
#include <string>

namespace re
{

    /**
     * @brief Returns tha last group match as out. Consumes the
     * match from the input string.
     * 
     * @param input 
     * @param rx 
     * @param out 
     * @return true 
     * @return false 
     */
    inline bool searchAndConsume(std::string* input,
        const std::regex& rx,
        std::string* out)
    {
        std::smatch mo;
        bool success = std::regex_search(*input, mo, rx);

        if (success)
        {
            *out = mo[mo.size()-1].str();
            *input = mo.suffix().str();
        }

        return success;
    }

    inline bool searchAndConsume(std::string* input,
        const std::regex& rx,
        double* out)
    {
        std::smatch mo;
        bool success = std::regex_search(*input, mo, rx);

        if (success)
        {
            *out = std::stod(mo[mo.size()-1].str());
            *input = mo.suffix().str();
        }

        return success;
    }

    inline bool searchAndConsume(std::string* input,
        const std::regex& rx,
        unsigned long* out)
    {
        std::smatch mo;
        bool success = std::regex_search(*input, mo, rx);

        if (success)
        {
            *out = std::stoull(mo[mo.size()-1].str());
            *input = mo.suffix().str();
        }

        return success;
    }

    inline bool searchAndConsume(std::string* input,
        const std::regex& rx)
    {
        std::smatch mo;
        bool success = std::regex_search(*input, mo, rx);

        if (success)
        {
            *input = mo.suffix().str();
        }

        return success;
    }

    inline bool partialMatch(const std::string& input,
        const std::regex& rx,
        std::string* out)
    {
        std::smatch mo;
        bool success = std::regex_search(input, mo, rx);

        if (success)
        {
            *out = mo[mo.size()-1].str();
        }

        return success;
    }

    inline bool partialMatch(const std::string& input,
        const std::regex& rx,
        double* out)
    {
        std::smatch mo;
        bool success = std::regex_search(input, mo, rx);

        if (success)
        {
            *out = std::stod(mo[mo.size()-1].str());
        }

        return success;
    }

    inline bool partialMatch(const std::string& input,
        const std::regex& rx,
        double* out1,
        double* out2)
    {
        std::smatch mo;
        bool success = std::regex_search(input, mo, rx);

        if (success)
        {
            *out1 = std::stod(mo[mo.size()-2].str());
            *out2 = std::stod(mo[mo.size()-1].str());
        }

        return success;
    }

}
