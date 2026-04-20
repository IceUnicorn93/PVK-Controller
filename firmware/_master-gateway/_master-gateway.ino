#include "..\shared\protocol.h"
#include "..\shared\spiMaster.h"

void setup() {
  Serial.begin(921600); //  1500000

  DoSpiSetup();

  delay(2000);

  // Serial-Buffer leeren – entfernt Rauschbytes vom Boot/Port-Öffnen
  while (Serial.available()) Serial.read();
}

void loop() {

  // Wait until Serial has recevied a SerialPacket
  while (Serial.available() < sizeof(SerialPacket))
    delayMicroseconds(1);

  // Parse the Serial Data in the SerialPacket
  SerialPacket packet;
  Serial.readBytes((char*)&packet, sizeof(SerialPacket));

  // Check the Checksum of the received Package (Checksum of Payload Data)
  uint8_t calculatedChecksum = calcChecksum(packet.payload, PayloadSize);
  if (calculatedChecksum != packet.checksum) {
    Serial.println(calculatedChecksum); //"Checksum Error! %n", 
    return; // Discard the packet if checksum is incorrect
  }

  // If, Checksum is correct, send Data to the correct slave (based on Board ID) 
  switch(packet.boardId)
  {
    case 1:
    {      
      memset(sendbuf, 0, sizeof(SerialPacket));
      memcpy(sendbuf, &packet, sizeof(SerialPacket));

      DoSpiTransmission(0);
      break;
    }
    // case 2:
    //{
    //   delay(1);
    //   break;
    //}
  }
}