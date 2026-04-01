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

            var t = new Task(() => SendDataToESP32());
            t.Start();
            
        }

        private void SendDataToESP32()
        {
            var rng = new Random();
            //var payload = new Structs.PayloadBoard2
            //{
            //    Target1Identification = 1234,
            //    Target1Speed = 25,
            //    Target1Altitude = 12.34f,
            //    Target1Angle = -120.53f,
            //    Target1EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

            //    Target2Identification = 1234,
            //    Target2Speed = 25,
            //    Target2Altitude = 12.34f,
            //    Target2Angle = -120.53f,
            //    Target2EstimatedTimeOfArrival = -7.26f,

            //    Target3Identification = 1234,
            //    Target3Speed = 25,
            //    Target3Altitude = 12.34f,
            //    Target3Angle = -120.53f,
            //    Target3EstimatedTimeOfArrival = -7.26f,

            //    Target4Identification = 1234,
            //    Target4Speed = 25,
            //    Target4Altitude = 12.34f,
            //    Target4Angle = -120.53f,
            //    Target4EstimatedTimeOfArrival = -7.26f,

            //    spare = new byte[180]
            //};

            var data = new Structs.PayloadBoard1
            {
                leds = new byte[137],
                brightness = 50,
                displays = new byte[] { 1, 2, 3, 4, 5 },
                spare = new byte[242 - 137 - 1 - 5]
            };
            for (int i = 0; i < 137; i++)
            {
                data.leds[i] = 1;
            }
            data.brightness = 10;

            data.displays[0] = 100;
            data.displays[1] = 200;
            data.displays[2] = 150;
            data.displays[3] = 250;
            data.displays[4] = 50;

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
                port.Open();

                port.DataReceived += (s, e) =>
                {
                    SerialPort p = (SerialPort)s;
                    int expectedSize = System.Runtime.InteropServices.Marshal.SizeOf(typeof(Structs.AnswerBoard1));

                    while (p.IsOpen && p.BytesToRead > 0)
                    {
                        _receiveBuffer.Add((byte)p.ReadByte());
                    }

                    if (_receiveBuffer.Count >= expectedSize)
                    {
                        byte[] answerBytes = _receiveBuffer.Take(expectedSize).ToArray();
                        _receiveBuffer.RemoveRange(0, expectedSize);

                        //convert byte array to string
                        var answerString = Encoding.UTF8.GetString(_receiveBuffer.ToArray());
                        Debug.WriteLine($"Rohdaten empfangen: {answerString}");

                        var answer = Structs.BytesToStruct<Structs.AnswerBoard1>(answerBytes);
                        Debug.WriteLine($"AnswerBoard1 empfangen: {answer.LoadButton}");
                    }
                };

                byte[] headerBytes = Structs.StructToBytes(payload);
                port.Write(headerBytes, 0, headerBytes.Length);

                // Auf ACK-Byte (0xAC) warten – ESP32 sendet es nach Verarbeitung
                port.ReadTimeout = 5000; // 5 Sekunden 

                var startTime = DateTime.Now;
                var endTime = startTime.AddSeconds(3).Ticks;
                while (DateTime.Now.Ticks < endTime)
                {
                    Thread.Sleep(100);
                }

                port.Close();
                this.Invoke(new Action(() => {this.Close(); }));
            }
        }

        // Puffer außerhalb des Handlers deklarieren (z.B. als Feld in der Klasse)
        private readonly List<byte> _receiveBuffer = new List<byte>();
    }
}
