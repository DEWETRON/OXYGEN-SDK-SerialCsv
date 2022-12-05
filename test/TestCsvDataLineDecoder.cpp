#include <boost/test/unit_test.hpp>

#include <string>
#include "serialcsvplugin/CsvDataLineDecoder.h"



BOOST_AUTO_TEST_CASE(test_valid_csv_rows)
{
    auto test_result = [](serialcsv::CsvDataLineDecoder &t)
    {
        double value;
        BOOST_CHECK(t.valid());
        BOOST_CHECK(t.size() == 4);

        BOOST_CHECK(t.at(0, value));
        BOOST_CHECK(value == 14.5);

        BOOST_CHECK(t.at(1, value));
        BOOST_CHECK(value == -32.4);

        BOOST_CHECK(t.at(2, value));
        BOOST_CHECK(value == 78.8);

        BOOST_CHECK(t.at(3, value));
        BOOST_CHECK(value == 79.5);
    };


    BOOST_TEST_MESSAGE("A valid test-line containing 4 space-separated values ending with carriage return and new line");
    {
        std::string test = "14.5, -32.4, 78.8, 79.5 \n\r";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }

    BOOST_TEST_MESSAGE("A valid csv-string containing 4 space-separated values ending with new line");
    {
        std::string test = "14.5, -32.4, 78.8, 79.5 \n";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    BOOST_TEST_MESSAGE("A valid csv-string containing 4 non-separated values ending with new line");
    {
        std::string test = "14.5,-32.4,78.8,79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    BOOST_TEST_MESSAGE("A valid csv-string containing 4 various-separated values ending with new line");
    {
        std::string test = "14.5,   -32.4,  78.8,  79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    BOOST_TEST_MESSAGE("A csv-string whithout new-line or carriage return");
    {
        std::string test = "14.5, -32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    BOOST_TEST_MESSAGE("A csv-string whithout new-line or carriage return and trailing white-spaces");
    {
        std::string test = "          14.5, -32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }
    BOOST_TEST_MESSAGE("A csv-string with semicolon-separator");
    {
        std::string test = "14.5; -32.4; 78.8; 79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }

    BOOST_TEST_MESSAGE("A csv-string with tab-separator");
    {
        std::string test = "14.5 \t -32.4 \t 78.8 \t 79.5\n";
        
        serialcsv::CsvDataLineDecoder t(test);
        test_result(t);
    }

    BOOST_TEST_MESSAGE("A csv-string with timestamp-data");
    {
        std::string test = "#t:1500, 14.5, -32.4, 78.8, 79.5 \n";

        serialcsv::CsvDataLineDecoder t(test);
        BOOST_CHECK(t.hasTimestamp());
        BOOST_CHECK(t.getTimestamp() == 1500);
        test_result(t);
    }

}


BOOST_AUTO_TEST_CASE(test_invalid_csv_rows)
{
    BOOST_TEST_MESSAGE("A csv-string containing unexpected non-digit at the beginning of the string");
    {
        std::string test = "a14.5, 32.4, 78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        BOOST_CHECK(!t.valid());
        BOOST_CHECK(t.size() == 0);
    }
    BOOST_TEST_MESSAGE("A csv-string containing unexpected non-digit between two numbers of the string");
    {
        std::string test = "14.5, adsf 32.4, zx78.8, 79.5";

        serialcsv::CsvDataLineDecoder t(test);
        BOOST_CHECK(!t.valid());
        BOOST_CHECK(t.size() == 0);
    }
}

