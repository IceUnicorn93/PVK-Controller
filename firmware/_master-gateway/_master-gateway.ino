#include "..\shared\protocol.h"
#include "..\shared\spiMaster.h"

void setup() {
  Serial.begin(115200);

  DoSpiSetup();

  delay(2000);
}

void loop() {

  // Wait until Serial has recevied a SerialPacket
  while (Serial.available() < sizeof(SerialPacket))
    delay(10);

  // Parse the Serial Data in the SerialPacket
  SerialPacket packet;
  Serial.readBytes((char*)&packet, sizeof(SerialPacket));

  // Check the Checksum of the received Package (Checksum of Payload Data)
  uint8_t calculatedChecksum = calcChecksum(packet.payload, PayloadSize);
  if (calculatedChecksum != packet.checksum) {
    Serial.println("Checksum Error!");
    return; // Discard the packet if checksum is incorrect
  }

  // If, Checksum is correct, send Data to the correct slave (based on Board ID) 
  switch(packet.boardId)
  {
    case 1:
    {      
      memset(sendbuf, 0, sizeof(SerialPacket));
      memcpy(sendbuf, &packet, sizeof(SerialPacket));

      DoSpiTransmission();

      //sending all received data from the slave to the serial monitor
      // for(int i = 0; i < sizeof(AnswerBoard1); i++)
      //   Serial.print(recvbuf[i]);
      // Serial.println("");

      // Serial.write(recvbuf, 244);
      break;
    }
    // case 2:
    //   delay(1);
    //   break;
    // case 3:
    //   delay(1);
    //   break;
    // case 4:
    //   delay(1);
    //   break;
    // case 5:
    //   delay(1);
    //   break;
    // case 6:
    //   delay(1);
    //   break;
  }
}