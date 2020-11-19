#include <catch2/catch.hpp>
#include <iostream>
#include <string>

#include "CsvNode.h"

struct Serial
{
    std::string written;
    std::string to_read;

    void write(const char *c, size_t length)
    {
        written = std::string(c, length);
    }

    int read()
    {
        if (to_read.empty())
        {
            return -1;
        }
        else
        {
            char ret = to_read.at(0);
            to_read.erase(0, 1);
            return ret;
        }
    }

    unsigned int available()
    {
        return to_read.size();
    }
};

struct Clock
{
    using milliseconds = long long;
    Clock()
    {
        offset = 0;
        tick = 0;
    }

    milliseconds now()
    {
        return tick - offset;
    }

    void reset()
    {
        offset = tick;
    }

    milliseconds offset;
    milliseconds tick;
};

SCENARIO("Test the Serial-Csv-Node Firmware")
{
    GIVEN("A CSV-Node Instance")
    {
        // Reset Registry
        csvnode::Registry::instance().clear();
        csvnode::SerialCsvNode<Clock, Serial> node("Test-Node", 100);
        auto &clock = node.getClock();
        auto &serial = node.getSerial();

        WHEN("5 Channels are registered")
        {
            size_t idx[5];
            for (int i = 0; i < 5; i++)
            {
                etl::string<10> name = "Channel-";
                csvnode::registerChannel(etl::to_string(i, name, true), &idx[i]);
            }

            REQUIRE(csvnode::Registry::instance().get("Channel-0").getName() == "Channel-0");
            REQUIRE(csvnode::Registry::instance()[idx[0]].getName() == "Channel-0");
            REQUIRE(csvnode::Registry::instance().get("Channel-1").getName() == "Channel-1");
            REQUIRE(csvnode::Registry::instance()[idx[1]].getName() == "Channel-1");
            REQUIRE(csvnode::Registry::instance().get("Channel-2").getName() == "Channel-2");
            REQUIRE(csvnode::Registry::instance()[idx[2]].getName() == "Channel-2");
            REQUIRE(csvnode::Registry::instance().get("Channel-3").getName() == "Channel-3");
            REQUIRE(csvnode::Registry::instance()[idx[3]].getName() == "Channel-3");
            REQUIRE(csvnode::Registry::instance().get("Channel-4").getName() == "Channel-4");
            REQUIRE(csvnode::Registry::instance()[idx[4]].getName() == "Channel-4");
            REQUIRE(csvnode::Registry::instance().getSize() == 5);

            THEN("Sample 5 Channels")
            {
                serial.written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }
                REQUIRE(serial.written == "0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Sample 5 Channels with Timestamp")
            {
                node.enablePrefixTimestamp();
                serial.written.clear();
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }
                REQUIRE(serial.written == "#t:100,0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Send Reset Clock command and Sample 5 Channels")
            {
                node.enablePrefixTimestamp();
                serial.to_read = "#t0\n";
                serial.written.clear();

                // Set a clock value
                clock.tick = 1500;

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(serial.written == "#t:100,0.00,0.00,0.00,0.00,0.00,\n");
            }
            AND_THEN("Set different positive values on each channel")
            {
                csvnode::Registry::instance().get("Channel-0").logValue(0.25);
                csvnode::Registry::instance().get("Channel-1").logValue(2.5);
                csvnode::Registry::instance().get("Channel-2").logValue(0.75);
                csvnode::Registry::instance().get("Channel-3").logValue(5);
                csvnode::Registry::instance().get("Channel-4").logValue(6.75);

                node.enablePrefixTimestamp();
                serial.written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(serial.written == "#t:100,0.25,2.50,0.75,5.00,6.75,\n");
            }
            AND_THEN("Set different negative values on each channel")
            {
                csvnode::Registry::instance().get("Channel-0").logValue(-0.25);
                csvnode::Registry::instance().get("Channel-1").logValue(-2.5);
                csvnode::Registry::instance().get("Channel-2").logValue(-0.75);
                csvnode::Registry::instance().get("Channel-3").logValue(-5);
                csvnode::Registry::instance().get("Channel-4").logValue(-6.75);

                node.enablePrefixTimestamp();
                serial.written.clear();

                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }

                    clock.tick++;
                }

                REQUIRE(serial.written == "#t:100,-0.25,-2.50,-0.75,-5.00,-6.75,\n");
            }
        }

        WHEN("Register one Channel whith meta-data")
        {
            csvnode::registerChannel("Channel-0")
                .setUnit("[A]")
                .setMin(-20)
                .setMax(23.2);

            THEN("Request Header-Data")
            {
                serial.to_read = "#h\n";
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }
                }

                REQUIRE(serial.written == "#h:Channel-0#min:-20.00#max:23.20#u:[A],\n");
            }
            AND_THEN("Add a second channel without meta-data and request header")
            {
                csvnode::registerChannel("Channel-1");

                serial.to_read = "#h\n";
                // Run until node writes data
                while (true)
                {
                    node.run();

                    if (!serial.written.empty())
                    {
                        break;
                    }
                }

                REQUIRE(serial.written == "#h:Channel-0#min:-20.00#max:23.20#u:[A],Channel-1,\n");
            }
        }
    }
}
