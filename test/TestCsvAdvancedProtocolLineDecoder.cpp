#include <catch2/catch.hpp>

#include "serialcsvplugin/CsvAdvancedProtocolLineDecoder.h"

SCENARIO("Test Header-Data")
{
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
        test_result(t);
    }

    GIVEN("A valid header-line with meta-data and whitespaces")
    {        
        std::string test = "#h:Ampere#r: -5   -  -2.2,Temperature#min: -20   #u:°C,Voltage,RMS#max:   102.5   \n";

        serialcsv::CsvAdvancedProtocolLineDecoder t(test);
        test_result(t);
    }
}
