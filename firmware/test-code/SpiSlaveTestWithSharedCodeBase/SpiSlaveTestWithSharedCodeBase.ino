#include "..\..\shared\spiSlave.h"

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  DoSpiSetup();

  Serial.println("SPI Slave bereit.");
}

void loop() {
  // put your main code here, to run repeatedly:
  spi_slave_transaction_t t = DoSpiTransmissionPrepparation();

  DoSpiTransmission(t);
}
