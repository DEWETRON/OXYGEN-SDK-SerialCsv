#include <catch2/catch.hpp>
#include <string>

#include "serialcsvplugin/CsvDataLineDecoder.h"

SCENARIO("Test valid CSV-Rows")
{
    auto test_result = [](serialcsv::CsvDataLineDecoder &t)
    {
        double value;
        REQUIRE(t.valid());
        REQUIRE(t.size() == 4);

        REQUIRE(t.at(0, value));
        REQUIRE(value == 14.5);

        REQUIRE(t.at(1, value));
        REQUIRE(value == -32.4);

        REQUIRE(t.at(2, value));
        REQUIRE(value == 78.8);

        REQUIRE(t.at(3, value));
        REQUIRE(value == 79.5);
    };

    GIVEN("A valid test-line containing 4 space-separated values ending with carriage return and new line")
    {
        std::string test = "14.5, -32.4, 78.8, 79.5 \n\r";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A valid csv-string containing 4 space-separated values ending with new line")
    {
        std::string test = "14.5, -32.4, 78.8, 79.5 \n";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A valid csv-string containing 4 non-separated values ending with new line")
    {
        std::string test = "14.5,-32.4,78.8,79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A valid csv-string containing 4 various-separated values ending with new line")
    {
        std::string test = "14.5,   -32.4,  78.8,  79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A csv-string whithout new-line or carriage return")
    {
        std::string test = "14.5, -32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A csv-string whithout new-line or carriage return and trailing white-spaces")
    {
        std::string test = "          14.5, -32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    GIVEN("A csv-string with semicolon-separator")
    {
        std::string test = "14.5; -32.4; 78.8; 79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }

    GIVEN("A csv-string with tab-separator")
    {
        std::string test = "14.5 \t -32.4 \t 78.8 \t 79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }

    GIVEN("A csv-string with timestamp-data")
    {
        std::string test = "#t:1500, 14.5, -32.4, 78.8, 79.5 \n";

        serialcsv::CsvDataLineDecoder t(test);
        REQUIRE(t.hasTimestamp());
        REQUIRE(t.getTimestamp() == 1500);
        test_result(t);
    }
}

SCENARIO("Test invalid CSV-Rows")
{
    GIVEN("A csv-string containing unexpected non-digit at the beginning of the string")
    {
        std::string test = "a14.5, 32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        REQUIRE_FALSE(t.valid());
        REQUIRE(t.size() == 0);
    }
    GIVEN("A csv-string containing unexpected non-digit between two numbers of the string")
    {
        std::string test = "14.5, adsf 32.4, zx78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        REQUIRE_FALSE(t.valid());
        REQUIRE(t.size() == 0);
    }
}
