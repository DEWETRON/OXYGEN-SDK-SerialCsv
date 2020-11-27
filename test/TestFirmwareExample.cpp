#include <catch2/catch.hpp>
#include <CsvNode.h>
#include <random>

#include "SerialMock.h"

class MockArduinoSerial
{
public:
    void begin(int)
    {
    }
};

MockArduinoSerial Serial;

long random(long min, long max)
{
    std::random_device rd;     
    std::mt19937 rng(rd());  
    std::uniform_int_distribution<int> uni(min,max); 
    return uni(rng);
}

using DefaultArduinoCsvNode = csvnode::SerialCsvNode<MockClock, MockSerial>;
DefaultArduinoCsvNode CsvNode;

// *********************************************************************************************
// THE ARDUINO SKETCH
// This tests ensures that the sketch compiles and setup and loop run at without exceptions
// *********************************************************************************************
#include "../example/example.ino"


SCENARIO("Test the Arduino-Sketch")
{
    GIVEN("The Default-Arduino Setup and loop functions")
    {
        THEN("Call setup")
        {
            setup();
        }
        AND_THEN("Call loop")
        {
            loop();
        }
    }
}
