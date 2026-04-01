const uint8_t PayloadSize = 242;
const uint8_t AnswerSize = 244;

/*
Payload for Boards
# | Size (Byze)
--|--------------
1 | 143
2 | 60
3 | 53
4 | ?
5 | ?
6 | ?

Answer for Boards
# | Size (Byze)
--|--------------
1 | 10
2 | 0
3 | 23
4 | ?
5 | ?
6 | ?
*/

//--------------------------------------------------------------------------------
// Header
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
// Board 2 (Upper Right / Target List)
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	int16_t Target1Identification;			// 2 Bytes
	float   Target1Altitude;				// 4 Bytes
	float   Target1EstimatedTimeOfArrival;	// 4 Bytes
	float   Target1Angle;					// 4 Bytes
	uint8_t Target1Speed;					// 1 Byte
	
	int16_t Target2Identification;			// 2 Bytes
	float   Target2Altitude;				// 4 Bytes
	float   Target2EstimatedTimeOfArrival;	// 4 Bytes
	float   Target2Angle;					// 4 Bytes
	uint8_t Target2Speed;					// 1 Byte
	
	int16_t Target3Identification;			// 2 Bytes
	float   Target3Altitude;				// 4 Bytes
	float   Target3EstimatedTimeOfArrival;	// 4 Bytes
	float   Target3Angle;					// 4 Bytes
	uint8_t Target3Speed;					// 1 Byte
	
	int16_t Target4Identification;			// 2 Bytes
	float   Target4Altitude;				// 4 Bytes
	float   Target4EstimatedTimeOfArrival;	// 4 Bytes
	float   Target4Angle;					// 4 Bytes
	uint8_t Target4Speed;					// 1 Byte
	//60 Bytes
	uint8_t spare[PayloadSize - 15 - 15 - 15 - 15] ; // Spare Data
} PayloadBoard2;
typedef struct __attribute__((packed)) {
	uint8_t spare[AnswerSize] ; // Spare Data
} AnswerBoard2;
//--------------------------------------------------------------------------------
// Board 3
//--------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	
	int TargetIdentification;
	uint8_t TargetIdentificationValid;
	int ProjectileSpeed;
	float Countdown;
	float IdealAngle;
	uint8_t Precision[4];
	uint8_t SelectedMode;
	
	float SetAngle;
	float CurrentAngle;
	uint8_t LockRotation;

	uint8_t ProjectileSpeedArm[10];
	int ChargedSpeed;

	uint8_t Energy[10];
	uint8_t SequenceAssistant[4];
	
	uint8_t spare[PayloadSize - 53]; 
} PayloadBoard3;
typedef struct __attribute__((packed)) {
	uint8_t AmingCalcButton1;
	uint8_t AmingCalcButton2;
	uint8_t AmingCalcButton3;
	uint8_t AmingCalcButton4;
	uint8_t AmingCalcButton5;
	uint8_t AmingCalcButton6;
	uint8_t AmingCalcButton7;
	uint8_t AmingCalcButton8;
	uint8_t AmingCalcButton9;
	uint8_t AmingCalcButton0;
	uint8_t AmingCalcButtonNext;	// Needed?
	uint8_t AmingCalcButtonClear;
	uint8_t AmingCalcMode;			// Needed?
	
	int GrobAngleDelta;
	int FeinAngleDelta;
	
	uint8_t Locked;
	
	uint8_t Arm1;
	uint8_t Arm2;
	uint8_t Arm3;
	uint8_t Arm4;
	
	uint8_t Fire;
	
	uint8_t spare[AnswerSize - 23] ; // Spare Data
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