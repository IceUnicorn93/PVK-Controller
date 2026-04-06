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
            var data = new Structs.PayloadBoard1
            {
                leds = new byte[137],
                brightness = 50,
                displays = new byte[] { 1, 2, 3, 4, 5 },
                spare = new byte[242 - 137 - 1 - 5]
            };
            for (int i = 0; i < 137; i++)
                data.leds[i] = 0;

            data.brightness = 10;

            data.displays[0] = 1;
            data.displays[1] = 0;
            data.displays[2] = 0;
            data.displays[3] = 0;
            data.displays[4] = 0;

            var dataBytes = Structs.StructToBytes(data);    

            var payload = new Structs.PacketHeader
            {
                boardId = 1,
                checksum = Structs.CalcChecksum(dataBytes, dataBytes.Length),
                payload = dataBytes
            };

            using (SerialPort port = new SerialPort("COM3", 115200))
            {
                port.DtrEnable = false;  // Verhindert Auto-Reset des ESP32 beim Öffnen
                port.RtsEnable = false;

                // Auf ACK-Byte (0xAC) warten – ESP32 sendet es nach Verarbeitung
                port.ReadTimeout = 5000; // 5 Sekunden 

                port.Open();

                var hasReceived = false;

                port.DataReceived += (s, e) =>
                {
                    SerialPort p = (SerialPort)s;
                    int expectedSize = 244; //System.Runtime.InteropServices.Marshal.SizeOf(typeof(Structs.AnswerBoard1));

                    while (p.IsOpen && p.BytesToRead > 0)
                    {
                        _receiveBuffer.Add((byte)p.ReadByte());
                    }

                    if (_receiveBuffer.Count >= expectedSize)
                    {

                        byte[] answerBytes = _receiveBuffer.Take(expectedSize).ToArray();
                        _receiveBuffer.RemoveRange(0, expectedSize);

                        //convert byte array to string
                        //var answerString = Encoding.UTF8.GetString(answerBytes.ToArray());
                        //Debug.WriteLine($"Rohdaten empfangen: {answerString}");


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


                        hasReceived = true;
                    }
                };

                int counter = 0;
                while (port.IsOpen)
                {
                    data.leds[counter] = 1;
                    counter++;

                    if(counter >= 137)
                    {
                        data.leds = new byte[137];
                        data.displays[1] += 1;
                        counter = 0;
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

                    dataBytes = Structs.StructToBytes(data);


                    payload = new Structs.PacketHeader
                    {
                        boardId = 1,
                        checksum = Structs.CalcChecksum(dataBytes, dataBytes.Length),
                        payload = dataBytes
                    };

                    byte[] headerBytes = Structs.StructToBytes(payload);
                    hasReceived = false;
                    port.Write(headerBytes, 0, headerBytes.Length);

                    // Warten bis eine komplette Antwort empfangen wurde (max. 5 Sekunden)
                    var timeout = Stopwatch.StartNew();
                    while (!hasReceived && timeout.ElapsedMilliseconds < 5000)
                    {
                        await Task.Delay(1);
                    }

                    if (!hasReceived)
                    {
                        Debug.WriteLine("Timeout: Keine Antwort vom ESP32 erhalten.");
                    }

                    await Task.Delay(10); // Kurze Verzögerung, um CPU-Last zu reduzieren
                }

                port.Close();
            }
        }

        // Puffer außerhalb des Handlers deklarieren (z.B. als Feld in der Klasse)
        private readonly List<byte> _receiveBuffer = new List<byte>();
    }
}
