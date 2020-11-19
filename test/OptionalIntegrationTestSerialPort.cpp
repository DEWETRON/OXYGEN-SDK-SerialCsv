#include <catch2/catch.hpp>
#include <chrono>

#include "serialcsvplugin/details/SerialPort.h"
#include "serialcsvplugin/CsvDecoder.h"

/**
 * For integration-Testing, there should only be one Port available
 * The Tests expect a Microcontroller on COM3 running the Arduino example-code
 */

SCENARIO("Test Enumerate Ports")
{
    GIVEN("A Serial-Port Instance")
    {
        serialcsv::SerialPort port;
        auto ports = port.enumaratePorts();

        auto it = std::find_if(ports.begin(), ports.end(), [](const auto &port) {
            return port == "COM3";
        });

        REQUIRE(it != ports.end());
    }
}

SCENARIO("Test reading a serial-line")
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    int cb_counter = 0;

    GIVEN("A Serial Port instance")
    {
        auto cb = [&](const std::string &line) {
            cb_counter++;
        };
        serialcsv::SerialPort port;

        WHEN("Opening Port and waiting on data")
        {
            port.start({"COM3", 115200}, cb);
            std::this_thread::sleep_for(2s);
            REQUIRE(port.isOpen());
            REQUIRE(cb_counter > 0);
        }
    }
}

SCENARIO("Opening an unaivailable port")
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    int cb_counter = 0;

    GIVEN("A Serial Port instance")
    {
        auto cb = [&](const std::string &) {
            cb_counter++;
        };
        serialcsv::SerialPort port;

        WHEN("Opening Port and waiting on data")
        {
            port.start({"COM999", 9600}, cb);
            std::this_thread::sleep_for(2s);
            REQUIRE_FALSE(port.isOpen());
            REQUIRE(cb_counter == 0);
        }
    }
}

SCENARIO("Integration-Tests")
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    auto test_result = [](serialcsv::CsvDecoder::CsvChannels &channels) {
        REQUIRE(channels[0]->getName() == "Temperature");
        REQUIRE_FALSE(channels[0]->hasMax());
        REQUIRE_FALSE(channels[0]->hasMin());
        REQUIRE_FALSE(channels[0]->hasUnit());

        REQUIRE(channels[1]->getName() == "Current");
        REQUIRE(channels[1]->hasMax());
        REQUIRE(channels[1]->hasMin());
        REQUIRE(channels[1]->hasUnit());
        REQUIRE(channels[1]->getMin() == -20.0);
        REQUIRE(channels[1]->getMax() == 23.2);
        REQUIRE(channels[1]->getUnit() == "[A]");

        REQUIRE(channels[2]->getName() == "Voltage");
        REQUIRE_FALSE(channels[2]->hasMax());
        REQUIRE_FALSE(channels[2]->hasMin());
        REQUIRE(channels[2]->hasUnit());
        REQUIRE(channels[2]->getUnit() == "[V]");
    };

    GIVEN("A CsvDecoder Instance")
    {
        serialcsv::CsvDecoder decoder;

        WHEN("Listening on Serial Port")
        {
            REQUIRE(decoder.getSerial().enumaratePorts().at(0) == "COM3");
            decoder.listenOnComPort({"COM3", 115200});
            std::this_thread::sleep_for(1s);
            REQUIRE(decoder.getSerial().isOpen());

            AND_THEN("Look at channels datasets")
            {
                // Acquire Data for two seconds
                std::this_thread::sleep_for(2s);

                REQUIRE(decoder.numberOfChannels() == 3);
                auto& channels = decoder.getChannels();

                for (auto& channel : channels)
                {
                    auto dataset = channel->getAndResetDataset();
                    REQUIRE(dataset.size() > 0);
                }
            }

            THEN("Reqeust Header-Data")
            {
                REQUIRE(decoder.requestHeader());
                REQUIRE(decoder.numberOfChannels() == 3);
                auto& channels = decoder.getChannels();
                test_result(channels);
            }
        }
    }
}
