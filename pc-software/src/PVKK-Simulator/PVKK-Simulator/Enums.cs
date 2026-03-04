using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PVKK_Simulator
{
    public enum ControllerPart
    {
        //Upper LEDs 5 Bits enough (21 LEDs | 31)
        UPPER_LED_Heat = 1000_1,
        UPPER_LED_KWG = 1000_2,
        UPPER_LED_PGM = 1000_3,
        UPPER_LED_HE = 1000_4,
        UPPER_LED_LASER = 1000_5,
        UPPER_LED_Clear = 1000_6,
        UPPER_LED_Loaded = 1000_7,
        UPPER_LED_PrimeReady1 = 1000_8,
        UPPER_LED_PrimeReady2 = 1000_9,
        UPPER_LED_PrimeReady3 = 1000_10,
        UPPER_LED_PrimeInProgress1 = 1000_11,
        UPPER_LED_PrimeInProgress2 = 1000_12,
        UPPER_LED_PrimeInProgress3 = 1000_13,
        Upper_LED_PrimeDone1 = 1000_14,
        Upper_LED_PrimeDone2 = 1000_15,
        Upper_LED_PrimeDone3 = 1000_16,
        UPPER_LED_Chamber = 1000_17,
        UPPER_LED_Target1 = 1000_18,
        UPPER_LED_Target2 = 1000_19,
        UPPER_LED_Target3 = 1000_20,
        UPPER_LED_Target4 = 1000_21,
        UPPER_LED_Energy = 1000_22,

        // Upper Buttons 4 Bits enough (14 Buttons | 16)
        UPPER_BTN_KWG = 1,
        UPPER_BTN_PGM = 2,
        UPPER_BTN_HE = 3,
        UPPER_BTN_LASER = 4,
        UPPER_BTN_Clear = 5,
        UPPER_BTN_Loaded = 6,
        UPPER_BTN_PrimeStart1 = 7,
        UPPER_BTN_PrimeStart2 = 8,
        UPPER_BTN_PrimeStart3 = 9,
        UPPER_BTN_TargetClear1 = 10,
        UPPER_BTN_TargetClear2 = 11,
        UPPER_BTN_TargetClear3 = 12,
        UPPER_BTN_TargetClear4 = 13,
        UPPER_BTN_Chamber = 14,
    }
}
