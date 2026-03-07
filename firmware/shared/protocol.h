const uint8_t PayloadSize = 250;
const uint8_t AnswerSize = 20;
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
    uint8_t boardId;         // Ziel-Board (1, 2, 3, ...)
    uint8_t checksum;
    uint8_t payload[PayloadSize];    // Die eigentlichen Daten
} SerialPacket;
//--------------------------------------------------------------------------------
// Board 1 (Upper Center)
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
    uint8_t leds[137];       // 0=Off, 1=Primary, 2=Secondary
    uint8_t brightness;      // 0-100
    uint8_t displays[5];     // 0-255, für u8g2 Displays
	uint8_t spare[PayloadSize - 137 - 1 - 5] ; // Spare Data
} PayloadBoard1;
typedef struct __attribute__((packed)) {
    uint8_t AmmoSelection;
	
	uint8_t LoadButton;
	
	uint8_t Prime1Button;
	uint8_t Prime2Button;
	uint8_t Prime3Button;
	
	uint8_t Target1Clear;
	uint8_t Target2Clear;
	uint8_t Target3Clear;
	uint8_t Target4Clear;
	
	uint8_t ChamberButton;
	
	uint8_t spare[AnswerSize - 10] ; // Spare Data
} AnswerBoard1;
//--------------------------------------------------------------------------------
// Board 2
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	uint8_t spare[PayloadSize] ; // Spare Data
} PayloadBoard2;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard2;
//--------------------------------------------------------------------------------
// Board 3
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	uint8_t spare[PayloadSize] ; // Spare Data
} PayloadBoard3;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard3;
//--------------------------------------------------------------------------------
// Board 4
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	uint8_t spare[PayloadSize] ; // Spare Data
} PayloadBoard4;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard4;
//--------------------------------------------------------------------------------
// Board 5
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	uint8_t spare[PayloadSize] ; // Spare Data
} PayloadBoard5;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard5;
//--------------------------------------------------------------------------------
// Board 6
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	uint8_t spare[PayloadSize] ; // Spare Data
} PayloadBoard6;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard6;
//--------------------------------------------------------------------------------
uint8_t calcChecksum(uint8_t* data, size_t len) {
  uint8_t sum = 0;
  for (size_t i = 0; i < len; i++)
    sum ^= data[i];
  return sum;
}