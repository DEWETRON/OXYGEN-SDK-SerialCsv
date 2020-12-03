#include <CsvNode.h>

// Create a Node with 10 Channels
using myCsvNodeType = csvnode::SerialCsvNode<csvnode::ArduinoClock, csvnode::ArduinoSerial, 10>;
myCsvNodeType myCsvNode;

void setup() {
  // put your setup code here, to run once:
  myCsvNode.begin("My-Node", 115200, 100);
  myCsvNode.registerChannel("Channel-1");
  myCsvNode.registerChannel("Channel-2");
  myCsvNode.registerChannel("Channel-3");
  myCsvNode.registerChannel("Channel-4");
  myCsvNode.registerChannel("Channel-5");
  myCsvNode.registerChannel("Channel-6");
  myCsvNode.registerChannel("Channel-7"); 
  myCsvNode.registerChannel("Channel-8");
  myCsvNode.registerChannel("Channel-9");
  myCsvNode.registerChannel("Channel-10");
}

  void loop() {
  // put your main code here, to run repeatedly:
  myCsvNode.run();
}
