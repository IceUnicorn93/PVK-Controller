//--------------------------------------------------------------------------------
// Displays and I2C Multiplexer
//--------------------------------------------------------------------------------
#include <Wire.h>
#include "TCA9548A.h"
#include <U8g2lib.h>

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
  display.clearBuffer();
  display.drawStr(38, 53, data); // 0 Left | 64 (Display Hight) - 42 (Font Height) => 22 / 2 = 11 -> 42 + 11 = 53
  display.sendBuffer();
}

void SetupDisplay(U8G2 &display, uint8_t channel)
{
  selectMuxChannel(channel);
  display.begin();
  display.setFont(u8g2_font_7Segments_26x42_mn);
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

void handleData(uint8_t* response) {
  int tolerance = 50;
  int mappedA0Read = map(analogRead(analogePin), 0, 4095, 0, 1023);

  int clearVal = 306;
  int heVal = 490;
  int laserVal = 666;
  int pgmVal = 815;
  int kwgVal = 930;

  if (mappedA0Read > clearVal - tolerance && mappedA0Read < clearVal + tolerance) response[0] = 1;
  else if (mappedA0Read > heVal - tolerance && mappedA0Read < heVal + tolerance) response[0] = 2;
  else if (mappedA0Read > laserVal - tolerance && mappedA0Read < laserVal + tolerance) response[0] = 3;
  else if (mappedA0Read > pgmVal - tolerance && mappedA0Read < pgmVal + tolerance) response[0] = 4;
  else if (mappedA0Read > kwgVal - tolerance && mappedA0Read < kwgVal + tolerance) response[0] = 5;
  else response[0] = 0x00;

  response[1] = digitalRead(loadPin);
  response[2] = digitalRead(prime3Pin);
  response[3] = digitalRead(prime2Pin);
  response[4] = digitalRead(prime1Pin);
  response[5] = digitalRead(clear1Pin);
  response[6] = digitalRead(clear2Pin);
  response[7] = digitalRead(clear3Pin);
  response[8] = digitalRead(clear4Pin);
  response[9] = digitalRead(chamberPin);
}

//--------------------------------------------------------------------------------
// WiFi-Data
//--------------------------------------------------------------------------------

#include <WiFi.h>
const char* ssid = "---";
const char* password = "---";
const char* hostname = "pvk.console";

// Has to be the same as the  C#-Programm (port = 5000)
const uint16_t listenPort = 5000;
WiFiServer server(listenPort);

//--------------------------------------------------------------------------------
// Neo Pixels
//--------------------------------------------------------------------------------

#include <Adafruit_NeoPixel.h>
#define PIN            23
#define NUMPIXELS      137
// 
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

//--------------------------------------------------------------------------------
// Setup Method
//--------------------------------------------------------------------------------

void setup()
{
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

  // Setup WiFi
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Start Server for Connection
  server.begin();
  server.setNoDelay(true);

  // Setup I2C, Multiplexer (Mux) and Displays
  Wire.begin();
  // optional faster: 400kHz if Display+Mux can do it, needs testing
  //Wire.setClock(100000);

  I2CMux.begin(Wire);
  I2CMux.closeAll();

  SetupDisplay(u8g2, 0);
  SetupDisplay(u8g2_2, 1);
  SetupDisplay(u8g2_3, 2);
  SetupDisplay(u8g2_4, 3);
  SetupDisplay(u8g2_5, 4);

  pixels.setPixelColor(0, 0, 255, 0);
  pixels.show();
}

void loop()
{
  WiFiClient client = server.available();
  if (!client) return;

  uint8_t buffer[143];
  int received = 0;

  const uint32_t timeoutMs = 5000;
  uint32_t lastDataMs = millis();

  while (client.connected() && received < 143) {
    int avail = client.available();
    if (avail > 0) {
      int toRead = min(avail, 143 - received);
      int n = client.readBytes(buffer + received, toRead);
      received += n;
      lastDataMs = millis();
    } else {
      if (millis() - lastDataMs > timeoutMs) {
        break;
      }
      delay(1);
    }
  }

  if (received == 143) {
    for(int i = 0; i < NUMPIXELS; i++)
        if(buffer[i] == 0) pixels.setPixelColor(i, 0);
        else if(buffer[i] == 1) pixels.setPixelColor(i, ledColors_1[i]);
        else  if(buffer[i] == 2) pixels.setPixelColor(i, ledColors_2[i]);
      
      // --- 10-Byte-Antwort an den Client senden ---
      uint8_t response[10];
      handleData(response);  // Füllt das Array direkt
      client.write(response, 10);
      client.flush();
      
      pixels.setBrightness(buffer[NUMPIXELS]);
      pixels.show();

      if(lastAmmo != buffer[138])
      {
        if (buffer[138] == 0) SetDisplayData(u8g2, 0, "");
        else if (buffer[138] == 1) SetDisplayData(u8g2, 0, "KWG");
        else if (buffer[138] == 2) SetDisplayData(u8g2, 0, "PGM");
        else if (buffer[138] == 3) SetDisplayData(u8g2, 0, "HE");
        else if (buffer[138] == 4) SetDisplayData(u8g2, 0, "Laser");
        lastAmmo = buffer[138];
      }
      if(lastKWG != buffer[139])
      {
        SetDisplayData(u8g2_2, 1, String(buffer[139]).c_str());
        lastKWG = buffer[139];
      }
      if(lastPGM != buffer[140])
      {
        SetDisplayData(u8g2_3, 2, String(buffer[140]).c_str());
        lastPGM = buffer[140];
      }
      if(lastHE != buffer[141])
      {
        SetDisplayData(u8g2_4, 3, String(buffer[141]).c_str());
        lastHE = buffer[141];
      }
      if(lastLaser != buffer[142])
      {
        SetDisplayData(u8g2_5, 4, String(buffer[142]).c_str());
        lastLaser = buffer[142];
      }
  } else {  }

  client.stop();
}