#include <catch2/catch.hpp>
#include <iostream>
#include <string>

#include "CsvNode.h"
#include "SerialMock.h"

SCENARIO("Test Firmware-Error Cases")
{
    GIVEN("A CSV-Node Instance")
    {
        csvnode::SerialCsvNode<MockClock, MockSerial, 10> node;
        node.begin("Test-Node", 115200, 100);
        auto &clock = node.getClock();
        auto &serial = node.getSerial();

        WHEN("Register 11 channels when only 10 Channels are possible")
        {
            etl::array<etl::string<10>, 11> channel_names;

            for (int i = 0; i < 10; i++)
            {
                etl::string<10> name = "Channel-";
                channel_names[i] = etl::to_string(i, name, true);
                node.registerChannel(channel_names[i].c_str());
            }

            etl::string<10> name = "Channel-";
            channel_names[10] = etl::to_string(11, name, true);
            REQUIRE_THROWS(node.registerChannel(channel_names[10].c_str()));
            REQUIRE(written == "Out of channels. Increase NUM_CHANNELS.");
        }
        AND_THEN("Access an invalid object by index")
        {
            REQUIRE_THROWS(node[10]);
            REQUIRE(written == "Out of range.");
        }
        AND_THEN("Get a channel by name which does not exist.")
        {
            REQUIRE_THROWS(node.getChannel("XYZ"));
            REQUIRE(written == "Channel does not exist.");
        }
    }
}
