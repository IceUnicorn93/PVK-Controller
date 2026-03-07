<Query Kind="Program">
  <NuGetReference>System.IO.Ports</NuGetReference>
  <Namespace>System.IO.Ports</Namespace>
  <Namespace>System.Runtime.InteropServices</Namespace>
</Query>

void Main()
{
	// Senden
	var header = new PacketHeader { boardId = 1, payloadLen = new byte[250] };
	var payload = new PayloadBoard1
	{
		leds = new byte[137],
		brightness = 80,
		displays = new byte[] { 42, 100, 0, 255, 12 }
	};

	header.checksum = CalcChecksum(StructToBytes(payload));
	header.payloadLen = StructToBytes(payload);
}

public static byte CalcChecksum(byte[] data)
{
	byte sum = 0;
	for (int i = 0; i < data.Length; i++)
		sum ^= data[i];
	return sum;
}

// Better performance
//public static byte CalcChecksum(ReadOnlySpan<byte> data)
//{
//	byte sum = 0;
//	foreach (var b in data)
//		sum ^= b;
//	return sum;
//}

// Struct → byte[]
byte[] StructToBytes<T>(T obj) where T : struct
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
T BytesToStruct<T>(byte[] arr) where T : struct
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


const byte PayloadSize = 250;
const byte AnswerSize = 20;

//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PacketHeader
{
	public byte boardId;
	public byte checksum;
	public byte[] payloadLen;
}
//--------------------------------------------------------------------------------
// Board 1 (Upper Center)
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard1
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 137)]
	public byte[] leds;
	public byte brightness;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
	public byte[] displays;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize - 137 - 1 - 5 - 1)]
	public byte[] spare;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct AnswerBoard1
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
// Board 2
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard2
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
	public byte[] spare;
}
struct AnswerBoard2
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
	public byte[] spare;
}
//--------------------------------------------------------------------------------
// Board 3
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard3
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
	public byte[] spare;
}
struct AnswerBoard3
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
	public byte[] spare;
}
//--------------------------------------------------------------------------------
// Board 4
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard4
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
	public byte[] spare;
}
struct AnswerBoard4
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
	public byte[] spare;
}
//--------------------------------------------------------------------------------
// Board 5
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard5
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
	public byte[] spare;
}
struct AnswerBoard5
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
	public byte[] spare;
}
//--------------------------------------------------------------------------------
// Board 6
//--------------------------------------------------------------------------------
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct PayloadBoard6
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = PayloadSize)]
	public byte[] spare;
}
struct AnswerBoard6
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = AnswerSize)]
	public byte[] spare;
}