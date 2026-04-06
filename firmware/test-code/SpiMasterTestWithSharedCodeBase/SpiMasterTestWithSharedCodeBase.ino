#include "..\..\shared\spiMaster.h"

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  DoSpiSetup();
}

void loop() {
  DoSpiTransmission();
}