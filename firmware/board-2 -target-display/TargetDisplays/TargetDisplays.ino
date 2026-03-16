#include "TCA9548A.h"
#include <U8g2lib.h>
#include "..\..\shared\protocol.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c0_t4_Angle(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c1_t4_Speed(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c2_t4_Alt(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c3_t4_ETA(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c4_t4_Ident(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c5_t3_Angle(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c6_t3_Speed(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m1_c7_t3_Alt(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c0_t3_ETA(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c1_t3_Ident(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c2_t2_Angle(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c3_t2_Speed(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c4_t2_Alt(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c5_t2_ETA(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c6_t2_Ident(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m2_c7_t1_Angle(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m3_c0_t1_Speed(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m3_c1_t1_Alt(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m3_c2_t1_ETA(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2_m3_c3_t1_Ident(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA); 

TCA9548A I2CMux_1(0x70);  // Erster Multiplexer
TCA9548A I2CMux_2(0x71);  // Zweiter Multiplexer
TCA9548A I2CMux_3(0x72);  // Dritter Multiplexer

// Variablen für die Displays
int     Target1Identification;
float   Target1Altitude;
float   Target1EstimatedTimeOfArrival;
float   Target1Angle;
uint8_t Target1Speed;

int     Target2Identification;
float   Target2Altitude;
float   Target2EstimatedTimeOfArrival;
float   Target2Angle;
uint8_t Target2Speed;

int     Target3Identification;
float   Target3Altitude;
float   Target3EstimatedTimeOfArrival;
float   Target3Angle;
uint8_t Target3Speed;

int     Target4Identification;
float   Target4Altitude;
float   Target4EstimatedTimeOfArrival;
float   Target4Angle;
uint8_t Target4Speed;

// Callback that skips U8X8_MSG_DELAY_MILLI – the SSD1306 UNIVISION driver fires
// three 100 ms delays per display for a reset that is not connected (U8X8_PIN_NONE).
// Skipping these saves ~300 ms × 20 displays = ~6 s during setup.
uint8_t u8x8_no_reset_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  if (msg == U8X8_MSG_DELAY_MILLI)
    return 1;  // skip – no physical reset pin is connected
  return u8x8_gpio_and_delay_arduino(u8x8, msg, arg_int, arg_ptr);
}

// Tracks the currently open MUX so only the active one needs to be closed on switch
static uint8_t activeMux = 0;
void selectMuxChannel(uint8_t channel, uint8_t mux)
{
  I2CMux_1.closeAll();
  I2CMux_2.closeAll();
  I2CMux_3.closeAll();

  if      (mux == 1) I2CMux_1.openChannel(channel);
  else if (mux == 2) I2CMux_2.openChannel(channel);
  else if (mux == 3) I2CMux_3.openChannel(channel);
}

void SetDisplayData(U8G2 &display, uint8_t channel, uint8_t mux, const char *data)
{
  selectMuxChannel(channel, mux);

  display.clearBuffer();
  display.drawStr(0, 13, data);
  display.sendBuffer();
}

void SetupDisplay(U8G2 &display, uint8_t channel, uint8_t mux)
{
  selectMuxChannel(channel, mux);
  display.getU8x8()->gpio_and_delay_cb = u8x8_no_reset_delay;
  display.begin();
  display.setFont(u8g2_font_6x12_tr);
}

void setup() {
  Serial.begin(115200);

  I2CMux_1.begin(Wire);
  I2CMux_1.closeAll();
  I2CMux_2.begin(Wire);
  I2CMux_2.closeAll();
  I2CMux_3.begin(Wire);
  I2CMux_3.closeAll();

  Wire.setClock(400000);  // 400 kHz Fast-Mode – reduces clearDisplay() per display from ~50 ms to ~12 ms

  SetupDisplay(u8g2_m1_c0_t4_Angle, 0, 1);
  SetupDisplay(u8g2_m1_c1_t4_Speed, 1, 1);
  SetupDisplay(u8g2_m1_c2_t4_Alt, 2, 1);
  SetupDisplay(u8g2_m1_c3_t4_ETA, 3, 1);
  SetupDisplay(u8g2_m1_c4_t4_Ident, 4, 1);

  SetupDisplay(u8g2_m1_c5_t3_Angle, 5, 1);
  SetupDisplay(u8g2_m1_c6_t3_Speed, 6, 1);
  SetupDisplay(u8g2_m1_c7_t3_Alt, 7, 1);
  SetupDisplay(u8g2_m2_c0_t3_ETA, 0, 2);
  SetupDisplay(u8g2_m2_c1_t3_Ident, 1, 2);

  SetupDisplay(u8g2_m2_c2_t2_Angle, 2, 2);
  SetupDisplay(u8g2_m2_c3_t2_Speed, 3, 2);
  SetupDisplay(u8g2_m2_c4_t2_Alt, 4, 2);
  SetupDisplay(u8g2_m2_c5_t2_ETA, 5, 2);
  SetupDisplay(u8g2_m2_c6_t2_Ident, 6, 2);

  SetupDisplay(u8g2_m2_c7_t1_Angle, 7, 2);
  SetupDisplay(u8g2_m3_c0_t1_Speed, 0, 3);
  SetupDisplay(u8g2_m3_c1_t1_Alt, 1, 3);
  SetupDisplay(u8g2_m3_c2_t1_ETA, 2, 3);
  SetupDisplay(u8g2_m3_c3_t1_Ident, 3, 3);
  
  SetDisplayData(u8g2_m1_c4_t4_Ident, 4, 1, "Ready");
}


void loop() {
  if (Serial.available() >= sizeof(PayloadBoard2)) {
    PayloadBoard2 payload;
    Serial.readBytes((char*)&payload, sizeof(PayloadBoard2));
    Serial.write(0xAC); // ACK: Daten empfangen und verarbeitet
    Serial.flush();
    SetDisplayData(u8g2_m1_c4_t4_Ident, 4, 1, "Data Send");
    // Use payload data here

    if(payload.Target1Identification != Target1Identification) {
      Target1Identification = payload.Target1Identification;
      SetDisplayData(u8g2_m3_c3_t1_Ident, 3, 3, String(Target1Identification).c_str());
    }
    if(payload.Target1Altitude != Target1Altitude) {
      Target1Altitude = payload.Target1Altitude;
      SetDisplayData(u8g2_m3_c1_t1_Alt, 1, 3, String(Target1Altitude).c_str());
    }
    if(payload.Target1EstimatedTimeOfArrival != Target1EstimatedTimeOfArrival) {
      Target1EstimatedTimeOfArrival = payload.Target1EstimatedTimeOfArrival;
      SetDisplayData(u8g2_m3_c2_t1_ETA, 2, 3, String(Target1EstimatedTimeOfArrival).c_str());
    }
    if(payload.Target1Angle != Target1Angle) {
      Target1Angle = payload.Target1Angle;
      SetDisplayData(u8g2_m2_c7_t1_Angle, 7, 2, String(Target1Angle).c_str());
    }
    if(payload.Target1Speed != Target1Speed) {
      Target1Speed = payload.Target1Speed;
      SetDisplayData(u8g2_m3_c0_t1_Speed, 0, 3, String(Target1Speed).c_str());
    }

    if(payload.Target3Identification != Target3Identification) {
      Target3Identification = payload.Target3Identification;
      SetDisplayData(u8g2_m2_c6_t2_Ident, 6, 2, String(Target3Identification).c_str());
    }
    if(payload.Target3Altitude != Target3Altitude) {
      Target3Altitude = payload.Target3Altitude;
      SetDisplayData(u8g2_m2_c4_t2_Alt, 4, 2, String(Target3Altitude).c_str());
    }
    if(payload.Target3EstimatedTimeOfArrival != Target3EstimatedTimeOfArrival) {
      Target3EstimatedTimeOfArrival = payload.Target3EstimatedTimeOfArrival;
      SetDisplayData(u8g2_m2_c5_t2_ETA, 5, 2, String(Target3EstimatedTimeOfArrival).c_str());
    }
    if(payload.Target3Angle != Target3Angle) {
      Target3Angle = payload.Target3Angle;
      SetDisplayData(u8g2_m2_c2_t2_Angle, 2, 2, String(Target3Angle).c_str());
    }
    if(payload.Target3Speed != Target3Speed) {
      Target3Speed = payload.Target3Speed;
      SetDisplayData(u8g2_m2_c3_t2_Speed, 3, 2, String(Target3Speed).c_str());
    }

    if(payload.Target2Identification != Target2Identification) {
      Target2Identification = payload.Target2Identification;
      SetDisplayData(u8g2_m2_c1_t3_Ident, 1, 2, String(Target2Identification).c_str());
    }
    if(payload.Target2Altitude != Target2Altitude) {
      Target2Altitude = payload.Target2Altitude;
      SetDisplayData(u8g2_m1_c7_t3_Alt, 7, 1, String(Target2Altitude).c_str());
    }
    if(payload.Target2EstimatedTimeOfArrival != Target2EstimatedTimeOfArrival) {
      Target2EstimatedTimeOfArrival = payload.Target2EstimatedTimeOfArrival;
      SetDisplayData(u8g2_m2_c0_t3_ETA, 0, 2, String(Target2EstimatedTimeOfArrival).c_str());
    }
    if(payload.Target2Angle != Target2Angle) {
      Target2Angle = payload.Target2Angle;
      SetDisplayData(u8g2_m1_c5_t3_Angle, 5, 1, String(Target2Angle).c_str());
    }
    if(payload.Target2Speed != Target2Speed) {
      Target2Speed = payload.Target2Speed;
      SetDisplayData(u8g2_m1_c6_t3_Speed, 6, 1, String(Target2Speed).c_str());
    }

    if(payload.Target4Identification != Target4Identification) {
      Target4Identification = payload.Target4Identification;
      SetDisplayData(u8g2_m1_c4_t4_Ident, 4, 1, String(Target4Identification).c_str());
    }
    if(payload.Target4Altitude != Target4Altitude) {
      Target4Altitude = payload.Target4Altitude;
      SetDisplayData(u8g2_m1_c2_t4_Alt, 2, 1, String(Target4Altitude).c_str());
    }
    if(payload.Target4EstimatedTimeOfArrival != Target4EstimatedTimeOfArrival) {
      Target4EstimatedTimeOfArrival = payload.Target4EstimatedTimeOfArrival;
      SetDisplayData(u8g2_m1_c3_t4_ETA, 3, 1, String(Target4EstimatedTimeOfArrival).c_str());
    }
    if(payload.Target4Angle != Target4Angle) {
      Target4Angle = payload.Target4Angle;
      SetDisplayData(u8g2_m1_c0_t4_Angle, 0, 1, String(Target4Angle).c_str());
    }
    if(payload.Target4Speed != Target4Speed) {
      Target4Speed = payload.Target4Speed;
      SetDisplayData(u8g2_m1_c1_t4_Speed, 1 , 1, String(Target4Speed).c_str());
    }
  }
}
