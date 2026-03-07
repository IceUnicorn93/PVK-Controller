//SLAVE

#include <ESP32SPISlave.h>
#include <../../shared/protocol.h>

ESP32SPISlave slave;

static constexpr size_t BUFFER_SIZE = 6;
uint8_t rxBuffer[BUFFER_SIZE];
uint8_t txBuffer[BUFFER_SIZE] = {'W', 'E', 'L', 'T', '!', 0x00};

void setup() {
  Serial.begin(115200);

  txBuffer[5] = calcChecksum(txBuffer, 5);

  slave.setDataMode(SPI_MODE0);
  slave.setQueueSize(1);
  slave.begin(VSPI);

  Serial.println("Slave bereit, warte auf Daten...");
}

void loop() {
  // Blocking transfer: wartet bis Master Daten sendet
  size_t received = slave.transfer(txBuffer, rxBuffer, BUFFER_SIZE);

  if (received > 0) {
    uint8_t expected = calcChecksum(rxBuffer, 5);

    Serial.print("Empfangen: ");
    for (int i = 0; i < 5; i++) Serial.printf("%c", rxBuffer[i]);

    Serial.println(rxBuffer[5] == expected ? " -> OK ✓" : " -> FEHLER ✗");
  }
}

