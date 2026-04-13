#include <../shared/protocol.h>
//--------------------------------------------------------------------------------
// SPI Slave
//--------------------------------------------------------------------------------
#include "..\shared\spiSlave.h"

//--------------------------------------------------------------------------------
// Displays and I2C Multiplexer
//--------------------------------------------------------------------------------
#include <Wire.h>
#include "TCA9548A.h"
#include <U8g2lib.h>
#include <../shared/14SegFont.h>

TCA9548A I2CMux;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_2(U8G2_R2, U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_3(U8G2_R2, U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_4(U8G2_R2, U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_5(U8G2_R2, U8X8_PIN_NONE);

static int8_t currentChannel = -1; 
static inline void selectMuxChannel(uint8_t channel)
{
  if (currentChannel == (int8_t)channel) return;
  I2CMux.closeAll();          // definierter Zustand (kostet 1 I2C-Write)
  I2CMux.openChannel(channel);
  currentChannel = channel;
}

void SetDisplayData(U8G2 &display, uint8_t channel, const char *data)
{
  selectMuxChannel(channel);
  int length = strlen(data); // Length of String, "A" => 1; "AB" => 2; "ABC" => 3, etc.
  int size = 27 * length;
  int fullSizeMinusSize = 128 - size;
  int pos = fullSizeMinusSize / 2;
  display.clearBuffer();
  display.drawStr(pos, 53, data);
  //display.drawStr(38, 53, data); // 0 Left
  //38 => | 26 (Font Width) * 2 => 52 | 128 (Display Width) - 52 => 76 -> 76 / 2 = 38 (For 2 Digits)
  //53 => | 64 (Display Hight) - 42 (Font Height) => 22 / 2 = 11 -> 42 + 11 = 53
  display.sendBuffer();
}

void SetupDisplay(U8G2 &display, uint8_t channel)
{
  selectMuxChannel(channel);
  display.begin();
  //display.setFont(u8g2_font_7Segments_26x42_mn);
  display.setFont(MySegmentFont46px);
}

uint8_t lastAmmo = 0;
uint8_t lastKWG = 0;
uint8_t lastPGM = 0;
uint8_t lastHE = 0;
uint8_t lastLaser = 0;

//--------------------------------------------------------------------------------
// Constant Button Pins and Reading of anoaloge Ammo selection 
//--------------------------------------------------------------------------------

const int analogePin = 36;
const int loadPin = 39;
const int prime1Pin = 34;
const int prime2Pin = 35;
const int prime3Pin = 32;
const int chamberPin = 33;
const int clear1Pin = 25;
const int clear2Pin = 26;
const int clear3Pin = 27;
const int clear4Pin = 13;

void handleData(AnswerBoard1* response) {
  int tolerance = 50;
  int mappedA0Read = map(analogRead(analogePin), 0, 4095, 0, 1023);

  int clearVal = 306;
  int heVal = 490;
  int laserVal = 666;
  int pgmVal = 815;
  int kwgVal = 930;

  if (mappedA0Read > clearVal - tolerance && mappedA0Read < clearVal + tolerance) response->AmmoSelection = 1;
  else if (mappedA0Read > heVal - tolerance && mappedA0Read < heVal + tolerance) response->AmmoSelection = 2;
  else if (mappedA0Read > laserVal - tolerance && mappedA0Read < laserVal + tolerance) response->AmmoSelection = 3;
  else if (mappedA0Read > pgmVal - tolerance && mappedA0Read < pgmVal + tolerance) response->AmmoSelection = 4;
  else if (mappedA0Read > kwgVal - tolerance && mappedA0Read < kwgVal + tolerance) response->AmmoSelection = 5;
  else response->AmmoSelection = 0x00;

  response->LoadButton = digitalRead(loadPin);
  response->Prime3Button = digitalRead(prime3Pin);
  response->Prime2Button = digitalRead(prime2Pin);
  response->Prime1Button = digitalRead(prime1Pin);
  response->Target1Clear = digitalRead(clear1Pin);
  response->Target2Clear = digitalRead(clear2Pin);
  response->Target3Clear = digitalRead(clear3Pin);
  response->Target4Clear = digitalRead(clear4Pin);
  response->ChamberButton = digitalRead(chamberPin);
}

//--------------------------------------------------------------------------------
// Neo Pixels
//--------------------------------------------------------------------------------

#include <Adafruit_NeoPixel.h>
#define PIN            17
#define NUMPIXELS      137

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int ledColors_1[NUMPIXELS]
{
  8321775,  // Loaded Color
  10740427, // Laser Active
  10740427, // HE Active
  16774818, // Clear Chamber Right
  16774818, // Clear Chamber Left
  16755001, // Heat Color 1
  16755001, // Heat Color 2
  16755001, // Heat Color 3
  16755001, // Heat Color 4
  16755001, // Heat Color 5
  16755001, // Heat Color 6
  16755001, // Heat Color 7
  16755001, // Heat Color 8
  16755001, // Heat Color 9
  16755001, // Heat Color 10
  10740427, // KWG Color
  10740427, // PGM Color
  8387312,  // Ready to Prime 1
  8387312,  // Ready to Prime 2
  8387312,  // Ready to Prime 3
  16564808, // Prime in Progress Bottom 1
  16564808, // Prime in Progress Bottom 2
  16564808, // Prime in Progress Bottom 3
  16564808, // Prime in Progress Bottom 4
  16564808, // Prime in Progress Bottom 5
  16564808, // Prime in Progress Bottom 6
  16564808, // Prime in Progress Bottom 7
  16564808, // Prime in Progress Bottom 8
  16564808, // Prime in Progress Bottom 9
  16564808, // Prime in Progress Bottom 10
  16564808, // Prime in Progress Bottom 11
  16564808, // Prime in Progress Bottom 12
  16564808, // Prime in Progress Bottom 13
  16564808, // Prime in Progress Bottom 14
  16564808, // Prime in Progress Bottom 15
  16564808, // Prime in Progress Bottom 16
  16564808, // Prime in Progress Bottom 17
  16564808, // Prime in Progress Bottom 18
  16564808, // Prime in Progress Bottom 19
  16564808, // Prime in Progress Bottom 20
  16564808, // Prime in Progress Bottom 21
  16564808, // Prime in Progress Bottom 22
  16564808, // Prime in Progress Bottom 23
  16564808, // Prime in Progress Bottom 24
  16564808, // Prime in Progress Bottom 25
  16564808, // Prime in Progress Bottom 26
  16564808, // Prime in Progress Bottom 27
  16564808, // Prime in Progress Bottom 28
  16564808, // Prime in Progress Bottom 29
  16564808, // Prime in Progress Bottom 30
  16564808, // Prime in Progress Bottom 31
  16564808, // Prime in Progress Bottom 32
  16564808, // Prime in Progress Bottom 33
  16564808, // Prime in Progress Middle 33
  16564808, // Prime in Progress Middle 32
  16564808, // Prime in Progress Middle 31
  16564808, // Prime in Progress Middle 30
  16564808, // Prime in Progress Middle 29
  16564808, // Prime in Progress Middle 28
  16564808, // Prime in Progress Middle 27
  16564808, // Prime in Progress Middle 26
  16564808, // Prime in Progress Middle 25
  16564808, // Prime in Progress Middle 24
  16564808, // Prime in Progress Middle 23
  16564808, // Prime in Progress Middle 22
  16564808, // Prime in Progress Middle 21
  16564808, // Prime in Progress Middle 20
  16564808, // Prime in Progress Middle 19
  16564808, // Prime in Progress Middle 18
  16564808, // Prime in Progress Middle 17
  16564808, // Prime in Progress Middle 16
  16564808, // Prime in Progress Middle 15
  16564808, // Prime in Progress Middle 14
  16564808, // Prime in Progress Middle 13
  16564808, // Prime in Progress Middle 12
  16564808, // Prime in Progress Middle 11
  16564808, // Prime in Progress Middle 10
  16564808, // Prime in Progress Middle 9
  16564808, // Prime in Progress Middle 8
  16564808, // Prime in Progress Middle 7
  16564808, // Prime in Progress Middle 6
  16564808, // Prime in Progress Middle 5
  16564808, // Prime in Progress Middle 4
  16564808, // Prime in Progress Middle 3
  16564808, // Prime in Progress Middle 2
  16564808, // Prime in Progress Middle 1
  16564808, // Prime in Progress Top 1 
  16564808, // Prime in Progress Top 2
  16564808, // Prime in Progress Top 3
  16564808, // Prime in Progress Top 4
  16564808, // Prime in Progress Top 5
  16564808, // Prime in Progress Top 6
  16564808, // Prime in Progress Top 7
  16564808, // Prime in Progress Top 8
  16564808, // Prime in Progress Top 9
  16564808, // Prime in Progress Top 10
  16564808, // Prime in Progress Top 11
  16564808, // Prime in Progress Top 12
  16564808, // Prime in Progress Top 13
  16564808, // Prime in Progress Top 14
  16564808, // Prime in Progress Top 15
  16564808, // Prime in Progress Top 16
  16564808, // Prime in Progress Top 17
  16564808, // Prime in Progress Top 18
  16564808, // Prime in Progress Top 19
  16564808, // Prime in Progress Top 20
  16564808, // Prime in Progress Top 21
  16564808, // Prime in Progress Top 22
  16564808, // Prime in Progress Top 23
  16564808, // Prime in Progress Top 24
  16564808, // Prime in Progress Top 25
  16564808, // Prime in Progress Top 26
  16564808, // Prime in Progress Top 27
  16564808, // Prime in Progress Top 28
  16564808, // Prime in Progress Top 29
  16564808, // Prime in Progress Top 30
  16564808, // Prime in Progress Top 31
  16564808, // Prime in Progress Top 32
  16564808, // Prime in Progress Top 33
  9566967,  // Prime Top Done
  9566967,  // Prime Middle Done
  9566967,  // Prime Bottom Done
  11074035, // Target 4 Selected
  11074035, // Target 3 Selected
  11074035, // Target 2 Selected
  11074035, // Target 1 Selected
  16578102, // Chamber Ready to activate
  16755001, // Energy 10
  16755001, // Energy 9
  16755001, // Energy 8
  16755001, // Energy 7
  16755001, // Energy 6
  16755001, // Energy 5
  16755001, // Energy 4
  16755001, // Energy 3
  16755001, // Energy 2
  16755001  // Energy 1
};

const int ledColors_2[NUMPIXELS]
{
  8321775,  // Loaded Color
  10740427, // Laser Active
  10740427, // HE Active
  16774818, // Clear Chamber Right
  16774818, // Clear Chamber Left
  16755001, // Heat Color 1
  16755001, // Heat Color 2
  16755001, // Heat Color 3
  16755001, // Heat Color 4
  16755001, // Heat Color 5
  16755001, // Heat Color 6
  16755001, // Heat Color 7
  16755001, // Heat Color 8
  16755001, // Heat Color 9
  16755001, // Heat Color 10
  10740427, // KWG Color
  10740427, // PGM Color
  8387312,  // Ready to Prime 1
  8387312,  // Ready to Prime 2
  8387312,  // Ready to Prime 3
  16564808, // Prime in Progress Bottom 1
  16564808, // Prime in Progress Bottom 2
  16564808, // Prime in Progress Bottom 3
  16564808, // Prime in Progress Bottom 4
  16564808, // Prime in Progress Bottom 5
  16564808, // Prime in Progress Bottom 6
  16564808, // Prime in Progress Bottom 7
  16564808, // Prime in Progress Bottom 8
  16564808, // Prime in Progress Bottom 9
  16564808, // Prime in Progress Bottom 10
  16564808, // Prime in Progress Bottom 11
  16564808, // Prime in Progress Bottom 12
  16564808, // Prime in Progress Bottom 13
  16564808, // Prime in Progress Bottom 14
  16564808, // Prime in Progress Bottom 15
  16564808, // Prime in Progress Bottom 16
  16564808, // Prime in Progress Bottom 17
  16564808, // Prime in Progress Bottom 18
  16564808, // Prime in Progress Bottom 19
  16564808, // Prime in Progress Bottom 20
  16564808, // Prime in Progress Bottom 21
  16564808, // Prime in Progress Bottom 22
  16564808, // Prime in Progress Bottom 23
  16564808, // Prime in Progress Bottom 24
  16564808, // Prime in Progress Bottom 25
  16564808, // Prime in Progress Bottom 26
  16564808, // Prime in Progress Bottom 27
  16564808, // Prime in Progress Bottom 28
  16564808, // Prime in Progress Bottom 29
  16564808, // Prime in Progress Bottom 30
  16564808, // Prime in Progress Bottom 31
  16564808, // Prime in Progress Bottom 32
  16564808, // Prime in Progress Bottom 33
  16564808, // Prime in Progress Middle 33
  16564808, // Prime in Progress Middle 32
  16564808, // Prime in Progress Middle 31
  16564808, // Prime in Progress Middle 30
  16564808, // Prime in Progress Middle 29
  16564808, // Prime in Progress Middle 28
  16564808, // Prime in Progress Middle 27
  16564808, // Prime in Progress Middle 26
  16564808, // Prime in Progress Middle 25
  16564808, // Prime in Progress Middle 24
  16564808, // Prime in Progress Middle 23
  16564808, // Prime in Progress Middle 22
  16564808, // Prime in Progress Middle 21
  16564808, // Prime in Progress Middle 20
  16564808, // Prime in Progress Middle 19
  16564808, // Prime in Progress Middle 18
  16564808, // Prime in Progress Middle 17
  16564808, // Prime in Progress Middle 16
  16564808, // Prime in Progress Middle 15
  16564808, // Prime in Progress Middle 14
  16564808, // Prime in Progress Middle 13
  16564808, // Prime in Progress Middle 12
  16564808, // Prime in Progress Middle 11
  16564808, // Prime in Progress Middle 10
  16564808, // Prime in Progress Middle 9
  16564808, // Prime in Progress Middle 8
  16564808, // Prime in Progress Middle 7
  16564808, // Prime in Progress Middle 6
  16564808, // Prime in Progress Middle 5
  16564808, // Prime in Progress Middle 4
  16564808, // Prime in Progress Middle 3
  16564808, // Prime in Progress Middle 2
  16564808, // Prime in Progress Middle 1
  16564808, // Prime in Progress Top 1 
  16564808, // Prime in Progress Top 2
  16564808, // Prime in Progress Top 3
  16564808, // Prime in Progress Top 4
  16564808, // Prime in Progress Top 5
  16564808, // Prime in Progress Top 6
  16564808, // Prime in Progress Top 7
  16564808, // Prime in Progress Top 8
  16564808, // Prime in Progress Top 9
  16564808, // Prime in Progress Top 10
  16564808, // Prime in Progress Top 11
  16564808, // Prime in Progress Top 12
  16564808, // Prime in Progress Top 13
  16564808, // Prime in Progress Top 14
  16564808, // Prime in Progress Top 15
  16564808, // Prime in Progress Top 16
  16564808, // Prime in Progress Top 17
  16564808, // Prime in Progress Top 18
  16564808, // Prime in Progress Top 19
  16564808, // Prime in Progress Top 20
  16564808, // Prime in Progress Top 21
  16564808, // Prime in Progress Top 22
  16564808, // Prime in Progress Top 23
  16564808, // Prime in Progress Top 24
  16564808, // Prime in Progress Top 25
  16564808, // Prime in Progress Top 26
  16564808, // Prime in Progress Top 27
  16564808, // Prime in Progress Top 28
  16564808, // Prime in Progress Top 29
  16564808, // Prime in Progress Top 30
  16564808, // Prime in Progress Top 31
  16564808, // Prime in Progress Top 32
  16564808, // Prime in Progress Top 33
  255,  // Prime Top Done
  255,  // Prime Middle Done
  255,  // Prime Bottom Done
  11074035, // Target 4 Selected
  11074035, // Target 3 Selected
  11074035, // Target 2 Selected
  11074035, // Target 1 Selected
  16578102, // Chamber Ready to activate
  16755001, // Energy 10
  16755001, // Energy 9
  16755001, // Energy 8
  16755001, // Energy 7
  16755001, // Energy 6
  16755001, // Energy 5
  16755001, // Energy 4
  16755001, // Energy 3
  16755001, // Energy 2
  16755001  // Energy 1
};

void setup() {
  Serial.begin(115200);

  pixels.setBrightness(10);
  pixels.setPixelColor(0, 255, 0, 0);
  pixels.show();

  // Set Button Pins
  pinMode(analogePin, INPUT);
  pinMode(prime1Pin, INPUT);
  pinMode(prime2Pin, INPUT);
  pinMode(prime3Pin, INPUT);
  pinMode(chamberPin, INPUT);
  pinMode(clear1Pin, INPUT);
  pinMode(clear2Pin, INPUT);
  pinMode(clear3Pin, INPUT);
  pinMode(clear4Pin, INPUT);
  pinMode(loadPin, INPUT);

  // Setup I2C, Multiplexer (Mux) and Displays
  Wire.begin();
  // optional faster: 400kHz if Display+Mux can do it, needs testing
  Wire.setClock(100000);

  I2CMux.begin(Wire);
  I2CMux.closeAll();

  SetupDisplay(u8g2, 0);
  SetupDisplay(u8g2_2, 1);
  SetupDisplay(u8g2_3, 2);
  SetupDisplay(u8g2_4, 3);
  SetupDisplay(u8g2_5, 4);

  DoSpiSetup();

  pixels.setPixelColor(0, 0, 255, 0);
  pixels.show();
}

void loop()
{
  AnswerBoard1 response;
  handleData(&response);
  memcpy(sendbuf, &response, sizeof(AnswerBoard1));

  DoSpiTransmission();

  // Cast received Data into SerialPacket
  SerialPacket packet; 
  memcpy(&packet, recvbuf, sizeof(SerialPacket));

  // Checksum prüfen – aber auch sicherstellen dass nicht alles Null ist (Falsch-Positiv)
  if(packet.checksum == calcChecksum(packet.payload, PayloadSize))
  {
    // cast payload into PayloadBoard1
    PayloadBoard1 payload;
    memcpy(&payload, packet.payload, sizeof(PayloadBoard1));

    // Decompress 35 bytes → 137 × 2-bit LED values and set pixel colors
    for(int i = 0; i < NUMPIXELS; i++)
    {
      uint8_t byteIndex = i / 4;
      uint8_t bitShift  = 6 - (i % 4) * 2;
      uint8_t value     = (payload.leds[byteIndex] >> bitShift) & 0b11;

      if(value == 1)      pixels.setPixelColor(i, ledColors_1[i]);
      else if(value == 2) pixels.setPixelColor(i, ledColors_2[i]);
      else                pixels.setPixelColor(i, 0);
    }
    
    pixels.setBrightness(payload.brightness);
    pixels.show();

    if(lastAmmo != payload.displays[0])
    {
      if (payload.displays[0] == 0) SetDisplayData(u8g2, 0, "");
      else if (payload.displays[0] == 1) SetDisplayData(u8g2, 0, "KWG");
      else if (payload.displays[0] == 2) SetDisplayData(u8g2, 0, "PGM");
      else if (payload.displays[0] == 3) SetDisplayData(u8g2, 0, "HE");
      else if (payload.displays[0] == 4) SetDisplayData(u8g2, 0, "Laser");
      lastAmmo = payload.displays[0];
    }
    if(lastKWG != payload.displays[1])
    {
      SetDisplayData(u8g2_2, 1, String(payload.displays[1]).c_str());
      lastKWG = payload.displays[1];
    }
    if(lastPGM != payload.displays[2])
    {
      SetDisplayData(u8g2_3, 2, String(payload.displays[2]).c_str());
      lastPGM = payload.displays[2];
    }
    if(lastHE != payload.displays[3])
    {
      SetDisplayData(u8g2_4, 3, String(payload.displays[3]).c_str());
      lastHE = payload.displays[3];
    }
    if(lastLaser != payload.displays[4])
    {
      SetDisplayData(u8g2_5, 4, String(payload.displays[4]).c_str());
      lastLaser = payload.displays[4];
    }
  }
}

