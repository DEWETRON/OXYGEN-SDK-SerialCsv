# A CSV-Node library for OXYGEN
This library implements the CSV-Protocol as specified by the corresponding OXYGEN-DEWETRTON Plugin. It is fully compliant with the protocol and can be used to send simple CSV-Data as well as complex header request to the OXYGEN Plugin. The library is compatible with the Arduino Library system as well as platform.io as it implements the necessary [folder-structure](https://arduino.github.io/arduino-cli/latest/library-specification/). In addition, it can be easily embedded and used in any other project. The only prerequisite is C++11 compiler. **In addition, the plugin makes the great Embedded-Template-Library (ETL) available to your project.** 

# Usage on Arduino

The following example summarizes the usage of the Plugin in a simple sketch:

```c++
#include <CsvNode.h>

// Create the Node
csvnode::ArduinoCsvNode node("Test-Node", 100);
size_t VOLTAGE = -1;

// Option 1: Keep a reference to the channel
csvnode::Channel& temperature = csvnode::registerChannel("Temperature");

void setup() {
  // put your setup code here, to run once:

  // Option 2: Simply register the channel and add optional metadata
  csvnode::registerChannel("Current")
  .setUnit("[A]")
  .setMin(-20)
  .setMax(23.2);

  // Option 3: Get the Identifier of the Channel during registration
  csvnode::registerChannel("Voltage", &VOLTAGE)
  .setUnit("[V]");

  // Enable optional Timestamp-Prefix
  node.enablePrefixTimestamp();

  // Activate the Serial-Port used by the ArduinoCsvNode
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Option 1: Access per Reference
  temperature.logValue(random(-10, 60));
  // Option 2: Access per Name
  csvnode::getChannel("Current").logValue(random(-20, 23.2));
  // Option 3: Access per Identifier
  csvnode::getChannel(VOLTAGE).logValue(random(0, 30));
  
  // Ensure node can do its work
  node.run();
}
```

# Implementing another Hardware-System

To customize hardware support, one must simply provide the following structures to the SerialCsvNode template:

```C++
struct CustomSerial
{
    void write(const char *c, size_t length)
    {
        // write the given bytes to the serial port
    }

    int read()
    {
        // read a single byte
        return byte;
    }

    unsigned int available()
    {
        // return the number of available bytes
        return available;
    }
};

struct CustomClock
{
    // Provide the actual datatype for milliseconds
    using milliseconds = unsigned long;
    Clock()
    {
        // Set offset to current time
        offset = now();
    }

    milliseconds now()
    {
        // Return monotonic increasing timepoint in milliseconds
        return millis() - offset;
    }

    void reset()
    {
        // Set offset to now() so that time now() begins at zero again
        offset = now();
    }
    
    // Store the offset
    milliseconds offset;
};

// Now the define the custom type
using CustomCsvNode = csvnode::SerialCsvNode<CustomClock, CustomSerial>;
```

# Memory and Customization

The library does not allocate any dynamic memory. Strings are stored in static allocated arrays. The memory-usage and other settings can be optimized by providing the following defines before including `<CsvNode.h>`:

- **CSVNODE_NUM_CHANNELS** 

  (default: 5) set the number of channels to be provided by the registry

- **CSVNODE_MAX_LENGTH_NAME_STRING** 

  (default: 20) the maximum length of the channel-names

- **CSVNODE_MAX_LENGTH_SHORT_STRING** 

  (default: 5) the maximum length of short strings like the unit metadata property

- **CSVNODE_MAX_LENGTH_CSV_STRING** 

  (default: MAX_LENGTH_NAME_STRING * NUM_CHANNELS) the maximum length of the internal buffer

- **CSVNODE_DECIMAL_PRECISION** 

  (default: 2) the precision decimal precision of the CSV-String

- **CSVNODE_EOL** 

  (default: "\n") the end of line character

- **CSVNODE_SEP** 

  (default: ",") the separator

For example, one can simply optimize memory and change the separator by providing the following defines:

```c++
#define CSVNODE_SEP ";"
#define CSVNODE_NUM_CHANNELS 3
#include <CsvNode.h>
...
```

Further information can be found in `CsvNodeProfile.h`.

# Dependencies

The library takes advantages of the [Embedded Template Library](https://www.etlcpp.com/) which becomes available to the programmer by including the project. If you already are using an up-to-date version of the ETL, make sure it is available on the compilers include path and remove it from the libraries folder. 


# To-Dos and Known Issues
- If there is a need from users of the library, dependency on ETL shall be made optional
- **The Plugin undefines `min` and `max` macro!** (But ETL implements them, so simply use `etl::min` and `etl::max`)

