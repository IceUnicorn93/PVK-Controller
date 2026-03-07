// MASTER

#include <SPI.h>
#include <../../shared/protocol.h>

#define CS_PIN 5

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  
  // SPI Einstellungen
  SPI.setFrequency(1000000);  // 1 MHz, langsam für Stabilität
  SPI.setDataMode(SPI_MODE0);
}

void loop() {
  uint8_t sendData[] = {'H', 'A', 'L', 'L', 'O', 0x00};  // Letztes Byte = Checksumme
  sendData[5] = calcChecksum(sendData, 5);
  
  uint8_t recvData[6];
  
  digitalWrite(CS_PIN, LOW);
  SPI.transferBytes(sendData, recvData, 6);
  digitalWrite(CS_PIN, HIGH);
  
  // Prüfen
  uint8_t expected = calcChecksum(recvData, 5);
  
  Serial.print("Gesendet: ");
  for (int i = 0; i < 5; i++) Serial.printf("%c", sendData[i]);
  
  Serial.print(" | Empfangen: ");
  for (int i = 0; i < 5; i++) Serial.printf("%c", recvData[i]);
  
  Serial.println(recvData[5] == expected ? " -> OK ✓" : " -> FEHLER ✗");
  
  delay(1000);
}
