#include <catch2/catch.hpp>

#include "serialcsvplugin/CsvAdvancedProtocolLineDecoder.h"

SCENARIO("Test Header-Data")
{
    auto test_result_with_underscores = [](serialcsv::CsvAdvancedProtocolLineDecoder &t) {
        REQUIRE(t.isAdvancedProtocolLine());
        REQUIRE(t.getMessageType() == serialcsv::MessageType::Header);

        auto channels = t.getChannelsMetadata();
        REQUIRE(channels[0].name == "Theta");
        REQUIRE(channels[0].hasMax);
        REQUIRE(channels[0].hasMin);
        REQUIRE(channels[0].hasUnit);
        REQUIRE(channels[0].min == -5);
        REQUIRE(channels[0].max == 20);
        REQUIRE(channels[0].unit == "rad");

        REQUIRE(channels[1].name == "Alpha");
        REQUIRE(!channels[1].hasMax);
        REQUIRE(channels[1].hasMin);
        REQUIRE(channels[1].hasUnit);
        REQUIRE(channels[1].min == -12.0);
        REQUIRE(channels[1].max == 0.0);
        REQUIRE(channels[1].unit == "rad");

        REQUIRE(channels[2].name == "Theta_d");
        REQUIRE(channels[2].hasMax);
        REQUIRE(channels[2].hasMin);
        REQUIRE(channels[2].hasUnit);
        REQUIRE(channels[2].min == -5);
        REQUIRE(channels[2].max == 20);
        REQUIRE(channels[2].unit == "rad/s");

        REQUIRE(channels[3].name == "Alpha_d");
        REQUIRE(!channels[3].hasMax);
        REQUIRE(channels[3].hasMin);
        REQUIRE(channels[3].hasUnit);
        REQUIRE(channels[3].min == -12.0);
        REQUIRE(channels[3].max == 0.0);
        REQUIRE(channels[3].unit == "rad/s");
    };

    auto test_result = [](serialcsv::CsvAdvancedProtocolLineDecoder &t) {
        REQUIRE(t.isAdvancedProtocolLine());
        REQUIRE(t.getMessageType() == serialcsv::MessageType::Header);

        auto channels = t.getChannelsMetadata();
        REQUIRE(channels[0].name == "Ampere");
        REQUIRE(channels[0].hasMax);
        REQUIRE(channels[0].hasMin);
        REQUIRE(!channels[0].hasUnit);
        REQUIRE(channels[0].min == -5);
        REQUIRE(channels[0].max == -2.2);
        REQUIRE(channels[0].unit == "");

        REQUIRE(channels[1].name == "Temperature");
        REQUIRE(!channels[1].hasMax);
        REQUIRE(channels[1].hasMin);
        REQUIRE(channels[1].hasUnit);
        REQUIRE(channels[1].min == -20.0);
        REQUIRE(channels[1].max == 0.0);
        REQUIRE(channels[1].unit == "°C");

        REQUIRE(channels[2].name == "Voltage");
        REQUIRE(!channels[2].hasMax);
        REQUIRE(!channels[2].hasMin);
        REQUIRE(!channels[2].hasUnit);
        REQUIRE(channels[2].min == 0.0);
        REQUIRE(channels[2].max == 0.0);
        REQUIRE(channels[2].unit == "");

        REQUIRE(channels[3].name == "RMS");
        REQUIRE(channels[3].hasMax);
        REQUIRE(!channels[3].hasMin);
        REQUIRE(!channels[3].hasUnit);
        REQUIRE(channels[3].min == 0.0);
        REQUIRE(channels[3].max == 102.5);
        REQUIRE(channels[3].unit == "");
    };

    GIVEN("A valid header-line with meta-data")
    {
        std::string test = "#h:Ampere#r:-5--2.2,Temperature#min:-20#u:°C,Voltage,RMS#max:102.5\n";

        serialcsv::CsvAdvancedProtocolLineDecoder t(test);
        REQUIRE(t.getNumberOfChannels() == 4);
        test_result(t);
    }

    GIVEN("A valid header-line with meta-data and whitespaces")
    {
        std::string test = "#h:Ampere#r: -5   -  -2.2,Temperature#min: -20   #u:°C,Voltage,RMS#max:   102.5   \n";

        serialcsv::CsvAdvancedProtocolLineDecoder t(test);
        REQUIRE(t.getNumberOfChannels() == 4);
        test_result(t);
    }

    GIVEN("A valid header-line, names have underscores, with meta-data and whitespaces")
    {
        std::string test = "#h:Theta#r:-5-20#u:rad,Alpha#min:-12#u:rad,Theta_d#r:-5-20#u:rad/s,Alpha_d#min:-12#u:rad/s\n";

        serialcsv::CsvAdvancedProtocolLineDecoder t(test);
        REQUIRE(t.getNumberOfChannels() == 4);
        test_result_with_underscores(t);
    }
}
