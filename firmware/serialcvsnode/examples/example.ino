#include <CsvNode.h>

// Create the Node
csvnode::ArduinoCsvNode node("Test-Node", 100);
size_t VOLTAGE = -1;

// Option 1: Keep a reference to the channel
csvnode::Channel& temperature = csvnode::registerChannel("Temperature");

void setup() {
  // put your setup code here, to run once:

  // Option 2: Simply register the channel
  csvnode::registerChannel("Current")
  .setUnit("[A]")
  .setMin(-20)
  .setMax(23.2);

  // Option 3: Get the Identifier of the Channel during registration
  csvnode::registerChannel("Voltage", &VOLTAGE)
  .setUnit("[V]");

  // Enable Timestamp-Prefix
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
