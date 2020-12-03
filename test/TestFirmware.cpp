#include <catch2/catch.hpp>
#include <iostream>
#include <string>

#include "CsvNode.h"
#include "SerialMock.h"

SCENARIO("Test the Serial-Csv-Node Firmware")
{
    GIVEN("A CSV-Node Instance")
    {
        // Reset Registry
        csvnode::SerialCsvNode<MockClock, MockSerial> node;
        node.begin("Test-Node", 115200, 100);
        auto &clock = node.getClock();
        auto &serial = node.getSerial();

        WHEN("5 Channels are registered")
        {
            size_t idx[5];
            etl::array<etl::string<10>, 5> channel_names;
            for (int i = 0; i < 5; i++)
            {
                etl::string<10> name = "Channel-";
                channel_names[i] = etl::to_string(i, name, true);
                node.registerChannel(channel_names[i].c_str(), &idx[i]);
            }

            REQUIRE(node.getChannel("Channel-0").getName() == "Channel-0");
            REQUIRE(node[idx[0]].getName() == "Channel-0");
            REQUIRE(node.getChannel("Channel-1").getName() == "Channel-1");
            REQUIRE(node[idx[1]].getName() == "Channel-1");
            REQUIRE(node.getChannel("Channel-2").getName() == "Channel-2");
            REQUIRE(node[idx[2]].getName() == "Channel-2");
            REQUIRE(node.getChannel("Channel-3").getName() == "Channel-3");
            REQUIRE(node[idx[3]].getName() == "Channel-3");
            REQUIRE(node.getChannel("Channel-4").getName() == "Channel-4");
            REQUIRE(node[idx[4]].getName() == "Channel-4");
            REQUIRE(node.getSize() == 5);

            THEN("Sample 5 Channels")
            {
                written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }
                REQUIRE(written == "0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Sample 5 Channels with Timestamp")
            {
                node.enablePrefixTimestamp();
                written.clear();
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }
                REQUIRE(written == "#t:100,0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Send Reset Clock command and Sample 5 Channels")
            {
                node.enablePrefixTimestamp();
                to_read = "#t0\n";
                written.clear();

                // Set a clock value
                clock.tick = 1500;

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(written == "#t:100,0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Set different positive values on each channel")
            {
                node.getChannel("Channel-0").logValue(0.25);
                node.getChannel("Channel-1").logValue(2.5);
                node.getChannel("Channel-2").logValue(0.75);
                node.getChannel("Channel-3").logValue(5);
                node.getChannel("Channel-4").logValue(6.75);

                node.enablePrefixTimestamp();
                written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(written == "#t:100,0.25,2.50,0.75,5.00,6.75,\n");
            }
            AND_THEN("Set different negative values on each channel")
            {
                node.getChannel("Channel-0").logValue(-0.25);
                node.getChannel("Channel-1").logValue(-2.5);
                node.getChannel("Channel-2").logValue(-0.75);
                node.getChannel("Channel-3").logValue(-5);
                node.getChannel("Channel-4").logValue(-6.75);

                node.enablePrefixTimestamp();
                written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(written == "#t:100,-0.25,-2.50,-0.75,-5.00,-6.75,\n");
            }
        }

        WHEN("Register one Channel whith meta-data")
        {
            node.registerChannel("Channel-0")
                .setUnit("[A]")
                .setMin(-20)
                .setMax(23.2);

            THEN("Request Header-Data")
            {
                to_read = "#h\n";
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }
                }

                REQUIRE(written == "#h:Channel-0#min:-20.00#max:23.20#u:[A],\n");
            }
            AND_THEN("Add a second channel without meta-data and request header")
            {
                node.registerChannel("Channel-1");

                to_read = "#h\n";
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!written.empty())
                    {
                        break;
                    }
                }

                REQUIRE(written == "#h:Channel-0#min:-20.00#max:23.20#u:[A],Channel-1,\n");
            }
        }
    }
}
