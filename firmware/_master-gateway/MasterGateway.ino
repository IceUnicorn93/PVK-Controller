#include "..\..\shared\protocol.h"
#include <SPI.h>

#define CS_PIN_Board1_CenterUpper 5

void setup() {
  Serial.begin(115200);

  SPI.begin();
  pinMode(CS_PIN_Board1_CenterUpper, OUTPUT);
  digitalWrite(CS_PIN_Board1_CenterUpper, HIGH);
  
  // SPI Einstellungen
  SPI.setFrequency(1000000);  // 1 MHz, langsam für Stabilität
  SPI.setDataMode(SPI_MODE0);

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
      //SPI Send the full package to the Slave
      // DMA-Transfer muss ein Vielfaches von 4 Bytes sein!
      const size_t TRANSFER_SIZE = (sizeof(SerialPacket) + 3) & ~3;  // = 244
      uint8_t sendData[TRANSFER_SIZE];
      memset(sendData, 0, TRANSFER_SIZE);         // Padding-Bytes auf 0
      memcpy(sendData, &packet, sizeof(SerialPacket));

      uint8_t recvData[TRANSFER_SIZE];
      memset(recvData, 0, sizeof(recvData));

      digitalWrite(CS_PIN_Board1_CenterUpper, LOW);
      SPI.transferBytes(sendData, recvData, TRANSFER_SIZE);
      digitalWrite(CS_PIN_Board1_CenterUpper, HIGH);

      //sending all received data from the slave to the serial monitor
      for(int i = 0; i < sizeof(AnswerBoard1); i++)
        Serial.print(recvData[i]);
      Serial.println("");
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