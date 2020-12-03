#include <CsvNode.h>

size_t VOLTAGE = -1;

// Option 1: Keep a reference to the channel
csvnode::Channel& temperature = CsvNode.registerChannel("Temperature");

void setup() {
  // put your setup code here, to run once:
  
  // Setup Node-Name, Baudrate and Sampling-Interval (on the default Arduino-Monitor-Port)
  CsvNode.begin("Test-Node", 115200, 100);

  // Option 2: Simply register the channel and access later by name
  CsvNode.registerChannel("Current")
  .setUnit("[A]")
  .setMin(-20)
  .setMax(23.2);

  // Option 3: Get the Identifier/Index of the Channel during registration
  CsvNode.registerChannel("Voltage", &VOLTAGE)
  .setUnit("[V]");

  // Enable Timestamp-Prefix
  CsvNode.enablePrefixTimestamp();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Option 1: Access per Reference
  temperature.logValue(random(-10, 60));
  // Option 2: Access per Name
  CsvNode.getChannel("Current").logValue(random(-20, 23.2));
  // Option 3: Access per Identifier
  CsvNode[VOLTAGE].logValue(random(0, 30));
  
  // Ensure node can do its work
  CsvNode.run();
}
