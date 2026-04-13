using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PVKK_Serial_Test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            var t = new Task(async () =>
            {
                await SendDataToESP32();
            });
            t.Start();
            
        }

        private async Task SendDataToESP32()
        {
            using (SerialPort port = new SerialPort("COM3", 921600)) // 1500000
            {
                port.DtrEnable = false;  // Verhindert Auto-Reset des ESP32 beim Öffnen
                port.RtsEnable = false;

                // Auf ACK-Byte (0xAC) warten – ESP32 sendet es nach Verarbeitung
                port.ReadTimeout = 5000; // 5 Sekunden 

                port.Open();

                var hasReceived = false;
                bool hasSent = false;

                port.DataReceived += (s, e) =>
                {
                    SerialPort p = (SerialPort)s;
                    int expectedSize = 62; //System.Runtime.InteropServices.Marshal.SizeOf(typeof(Structs.AnswerBoard1));

                    while (p.IsOpen && p.BytesToRead > 0)
                    {
                        _receiveBuffer.Add((byte)p.ReadByte());
                    }

                    if (_receiveBuffer.Count >= expectedSize)
                    {
                        hasReceived = true;

                        byte[] answerBytes = _receiveBuffer.Take(expectedSize).ToArray();
                        _receiveBuffer.RemoveRange(0, expectedSize);

                        //convert byte array to string
                        //var answerString = Encoding.UTF8.GetString(answerBytes.ToArray());
                        //Debug.WriteLine($"{answerString}");


                        //var answer = Structs.BytesToStruct<Structs.AnswerBoard1>(answerBytes);
                        //Debug.WriteLine($"AnswerBoard1 empfangen: {answer.LoadButton}");
                        //label1.Invoke(new Action(() =>
                        //{
                        //    label1.Text = $"Ammo: {answer.AmmoSelection}," +
                        //    $"Load: {answer.LoadButton}," +
                        //    $"Prime1: {answer.Prime1Button}," +
                        //    $"Prime2: {answer.Prime2Button}," +
                        //    $"Prime3: {answer.Prime3Button}";
                        //}) );
                    }
                };


                var leds = new byte[137];
                byte[] headerBytes = new byte[1];
                byte[] compress()
                {
                    // Kapazität vorab berechnen: ceil(137 / 4) = 35
                    int outputLength = (leds.Length + 3) / 4;
                    var result = new byte[outputLength]; // leds statt List<byte>


                    // Span<byte> vermeidet Bounds-Checks und Heap-Allokationen
                    ReadOnlySpan<byte> span = leds;

                    int idx = 0;
                    int fullChunks = leds.Length / 4;

                    // Volle 4er-Blöcke – direkter Indexzugriff statt LINQ
                    for (int i = 0; i < fullChunks; i++)
                    {
                        int offset = i * 4;
                        result[idx++] = (byte)(
                            ((span[offset] & 0b11) << 6) |
                            ((span[offset + 1] & 0b11) << 4) |
                            ((span[offset + 2] & 0b11) << 2) |
                             (span[offset + 3] & 0b11)
                        );
                    }

                    // Rest-Bytes behandeln (hier: 137 % 4 = 1 Byte)
                    int remaining = leds.Length % 4;
                    if (remaining > 0)
                    {
                        int offset = fullChunks * 4;
                        byte compressed = 0;
                        for (int j = 0; j < remaining; j++)
                        {
                            compressed |= (byte)((span[offset + j] & 0b11) << (6 - j * 2));
                        }
                        result[idx] = compressed;
                    }

                    return result;
                }

                int counter = -1;


                List<byte[]> dataToSend = new List<byte[]>();

                var data = new Structs.PayloadBoard1
                {
                    leds = new byte[137],
                    brightness = 50,
                    displays = new byte[] { 1, 2, 3, 4, 5 },
                    spare = new byte[60 - 35 - 1 - 5]
                };
                for (int i = 0; i < 137; i++)
                    data.leds[i] = 0;

                data.brightness = 10;

                data.displays[0] = 1;
                data.displays[1] = 0;
                data.displays[2] = 0;
                data.displays[3] = 0;
                data.displays[4] = 0;

                var payload = new Structs.PacketHeader();

                while (port.IsOpen && counter < 100)
                {
                    counter++;
                    leds[counter] = 1;
                    data.leds = compress();

                    var dataBytes = Structs.StructToBytes(data);

                    if (counter >= 136)
                    {
                        leds = new byte[137];
                        data.displays[1] += 1;
                        counter = -1;
                    }
                    if (data.displays[1] == 255)
                    {
                        data.displays[1] = 0;
                        data.displays[2] += 1;
                    }
                    if (data.displays[2] == 255)
                    {
                        data.displays[2] = 0;
                        data.displays[3] += 1;
                    }
                    if (data.displays[3] == 255)
                    {
                        data.displays[3] = 0;
                        data.displays[4] += 1;
                    }

                    payload = new Structs.PacketHeader
                    {
                        boardId = 1,
                        checksum = Structs.CalcChecksum(dataBytes, dataBytes.Length),
                        payload = dataBytes
                    };

                    headerBytes = Structs.StructToBytes(payload);

                    dataToSend.Add(headerBytes);

                    await Task.Delay(new TimeSpan((long)10)); // Kurze Verzögerung, um CPU-Last zu reduzieren 
                }


                var sw = Stopwatch.StartNew();
                var waitingTime = new TimeSpan((long)10_000);
                while (port.IsOpen && dataToSend.Count > 0)
                {
                    hasReceived = false;

                    if(dataToSend.Count > 0)
                    {
                        var swSend = Stopwatch.StartNew();
                        var headerBytesToSend = dataToSend[0];
                        port.Write(headerBytesToSend, 0, headerBytesToSend.Length);
                        dataToSend.RemoveAt(0);
                        swSend.Stop();
                        Debug.WriteLine($"Daten gesendet (Dauer: {swSend.ElapsedTicks} ticks), verbleibende Pakete: {dataToSend.Count}");
                    }
                    

                    // Warten bis eine komplette Antwort empfangen wurde (max. 5 Sekunden)
                    //var timeout = Stopwatch.StartNew();
                    //while (!hasReceived && timeout.ElapsedMilliseconds < 5000)
                    //{
                    //    await Task.Delay(1);
                    //}

                    //if (!hasReceived)
                    //{
                    //    Debug.WriteLine("Timeout: Keine Antwort vom ESP32 erhalten.");
                    //}

                    await Task.Delay(waitingTime); // Kurze Verzögerung, um CPU-Last zu reduzieren || 
                }
                sw.Stop();

                port.Close();
            }
        }

        // Puffer außerhalb des Handlers deklarieren (z.B. als Feld in der Klasse)
        private readonly List<byte> _receiveBuffer = new List<byte>();
    }
}
