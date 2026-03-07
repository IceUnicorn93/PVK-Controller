#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// SH1106 128x64 I2C
// Pins für ESP32 standardmäßig: SDA=21, SCL=22
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const size_t FRAME_SIZE = 128 * 64 / 8; // 1024 Bytes

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void loop() {
  static uint8_t frame[FRAME_SIZE];
  size_t received = 0;

  // Warte bis 1024 Bytes komplett da sind
  while (received < FRAME_SIZE) {
    if (Serial.available()) {
      frame[received++] = (uint8_t)Serial.read();
    }
  }

  // In Display-Buffer kopieren
  memcpy(u8g2.getBufferPtr(), frame, FRAME_SIZE);

  // Display aktualisieren
  u8g2.sendBuffer();
}