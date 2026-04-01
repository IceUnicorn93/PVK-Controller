using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PVKK_Serial_Test
{
    public static class Structs
    {
        // Struct → byte[]
        public static byte[] StructToBytes<T>(T obj) where T : struct
        {
            int size = Marshal.SizeOf<T>();
            byte[] arr = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(obj, ptr, true);
                Marshal.Copy(ptr, arr, 0, size);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
            return arr;
        }

        // byte[] → Struct
        public static T BytesToStruct<T>(byte[] arr) where T : struct
        {
            int size = Marshal.SizeOf<T>();
            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.Copy(arr, 0, ptr, size);
                return Marshal.PtrToStructure<T>(ptr);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }


        const byte PayloadSize = 242;
        const byte AnswerSize = 244;

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
        [StructLayout(LayoutKind.Sequential, Pack = 1, Size = PayloadSize + 2)]
        public struct PacketHeader
        {
            public byte boardId;
            public byte checksum;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
            public byte[] payload;
        }
        //--------------------------------------------------------------------------------
        // Board 1 (Upper Center)
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard1
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 137)]
            public byte[] leds;
            public byte brightness;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
            public byte[] displays;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize - 137 - 1 - 5)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard1
        {
            public byte AmmoSelection;

            public byte LoadButton;

            public byte Prime1Button;
            public byte Prime2Button;
            public byte Prime3Button;

            public byte Target1Clear;
            public byte Target2Clear;
            public byte Target3Clear;
            public byte Target4Clear;

            public byte ChamberButton;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize - 10)]
            public byte[] spare;
        }
        //--------------------------------------------------------------------------------
        // Board 2 (Upper Right / Target List)
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard2
        {
            public short Target1Identification;         // 2 Bytes (int16_t)
            public float Target1Altitude;               // 4 Bytes
            public float Target1EstimatedTimeOfArrival; // 4 Bytes
            public float Target1Angle;                  // 4 Bytes
            public byte Target1Speed;                   // 1 Byte

            public short Target2Identification;         // 2 Bytes (int16_t)
            public float Target2Altitude;               // 4 Bytes
            public float Target2EstimatedTimeOfArrival; // 4 Bytes
            public float Target2Angle;                  // 4 Bytes
            public byte Target2Speed;                   // 1 Byte

            public short Target3Identification;         // 2 Bytes (int16_t)
            public float Target3Altitude;               // 4 Bytes
            public float Target3EstimatedTimeOfArrival; // 4 Bytes
            public float Target3Angle;                  // 4 Bytes
            public byte Target3Speed;                   // 1 Byte

            public short Target4Identification;         // 2 Bytes (int16_t)
            public float Target4Altitude;               // 4 Bytes
            public float Target4EstimatedTimeOfArrival; // 4 Bytes
            public float Target4Angle;                  // 4 Bytes
            public byte Target4Speed;                   // 1 Byte

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize - 15 - 15 - 15 - 15)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard2
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
            public byte[] spare;
        }
        //--------------------------------------------------------------------------------
        // Board 3 (Lower Center)
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard3
        {
            public int TargetIdentification;        // 2 Byte
            public byte TargetIdentificationValid;  // 1 Byte
            public int ProjectileSpeed;             // 2 Byte
            public float Countdown;                 // 4 Byte
            public float IdealAngle;                // 4 Byte
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] Precision;                // 4 Byte
            public byte SelectedMode;               // 1 Byte

            public float SetAngle;                  // 4 Byte
            public float CurrentAngle;              // 4 Byte
            public byte LockRotation;               // 1 Byte

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
            public byte[] ProjectileSpeedArm;       // 10 Byte
            public int ChargedSpeed;                // 2 Byte

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
            public byte[] Energy;                   // 10 Byte
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] SequenceAssistant;        // 4 Byte

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize - 53)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard3
        {
            public byte AmingCalcButton1;
            public byte AmingCalcButton2;
            public byte AmingCalcButton3;
            public byte AmingCalcButton4;
            public byte AmingCalcButton5;
            public byte AmingCalcButton6;
            public byte AmingCalcButton7;
            public byte AmingCalcButton8;
            public byte AmingCalcButton9;
            public byte AmingCalcButton0;
            public byte AmingCalcButtonNext;    // Needed?
            public byte AmingCalcButtonClear;
            public byte AmingCalcMode;          // Needed?

            public int GrobAngleDelta;
            public int FeinAngleDelta;

            public byte Locked;

            public byte Arm1;
            public byte Arm2;
            public byte Arm3;
            public byte Arm4;

            public byte Fire;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize - 19 - 2 - 2)]
            public byte[] spare;
        }
        //--------------------------------------------------------------------------------
        // Board 4
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard4
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard4
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
            public byte[] spare;
        }
        //--------------------------------------------------------------------------------
        // Board 5
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard5
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard5
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
            public byte[] spare;
        }
        //--------------------------------------------------------------------------------
        // Board 6
        //--------------------------------------------------------------------------------
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct PayloadBoard6
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
            public byte[] spare;
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AnswerBoard6
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
            public byte[] spare;
        }


        public static byte CalcChecksum(byte[] data, int len)
        {
            byte sum = 0;
            for (int i = 0; i < len; i++)
                sum ^= data[i];
            return sum;
        }
    }
}
