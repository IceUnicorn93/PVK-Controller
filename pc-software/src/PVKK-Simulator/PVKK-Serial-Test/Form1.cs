using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
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
            while (true)
            {
                var rng = new Random();
                var payload = new Structs.PayloadBoard2
                {
                    Target1Identification = 1234,
                    Target1Speed = 25,
                    Target1Altitude = 12.34f,
                    Target1Angle = -120.53f,
                    Target1EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

                    Target2Identification = 1234,
                    Target2Speed = 25,
                    Target2Altitude = 12.34f,
                    Target2Angle = -120.53f,
                    Target2EstimatedTimeOfArrival = -7.26f,

                    Target3Identification = 1234,
                    Target3Speed = 25,
                    Target3Altitude = 12.34f,
                    Target3Angle = -120.53f,
                    Target3EstimatedTimeOfArrival = -7.26f,

                    Target4Identification = 1234,
                    Target4Speed = 25,
                    Target4Altitude = 12.34f,
                    Target4Angle = -120.53f,
                    Target4EstimatedTimeOfArrival = -7.26f,

                    spare = new byte[180]
                };

                //var payload = new Structs.PayloadBoard2
                //{
                //    Target1Identification = (short)rng.Next(1000, 9999),
                //    Target1Speed = (byte)rng.Next(0, 255),
                //    Target1Altitude = (float)(rng.NextDouble() * 10000),
                //    Target1Angle = (float)(rng.NextDouble() * 360 - 180),
                //    Target1EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

                //    Target2Identification = (short)rng.Next(1000, 9999),
                //    Target2Speed = (byte)rng.Next(0, 255),
                //    Target2Altitude = (float)(rng.NextDouble() * 10000),
                //    Target2Angle = (float)(rng.NextDouble() * 360 - 180),
                //    Target2EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

                //    Target3Identification = (short)rng.Next(1000, 9999),
                //    Target3Speed = (byte)rng.Next(0, 255),
                //    Target3Altitude = (float)(rng.NextDouble() * 10000),
                //    Target3Angle = (float)(rng.NextDouble() * 360 - 180),
                //    Target3EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

                //    Target4Identification = (short)rng.Next(1000, 9999),
                //    Target4Speed = (byte)rng.Next(0, 255),
                //    Target4Altitude = (float)(rng.NextDouble() * 10000),
                //    Target4Angle = (float)(rng.NextDouble() * 360 - 180),
                //    Target4EstimatedTimeOfArrival = (float)(rng.NextDouble() * 60),

                //    spare = new byte[180]
                //};

                using (SerialPort port = new SerialPort("COM5", 115200))
                {
                    port.DtrEnable = false;  // Verhindert Auto-Reset des ESP32 beim Öffnen
                    port.RtsEnable = false;
                    port.Open();
                    byte[] headerBytes = Structs.StructToBytes(payload);
                    port.Write(headerBytes, 0, headerBytes.Length);

                    // Auf ACK-Byte (0xAC) warten – ESP32 sendet es nach Verarbeitung
                    port.ReadTimeout = 5000; // 5 Sekunden Timeout
                    bool ackReceived = false;
                    try
                    {
                        int response = port.ReadByte();
                        Debug.WriteLine(response);
                        Debug.WriteLine((int)0xAC);
                        ackReceived = (response == 0xAC);
                    }
                    catch (TimeoutException)
                    {
                        Debug.WriteLine("FEHLER: Kein ACK vom ESP32 empfangen (Timeout)");
                    }

                    if (ackReceived)
                        Debug.WriteLine("ACK empfangen – ESP32 hat Daten verarbeitet.");
                    else
                        Debug.WriteLine("WARNUNG: Unerwartete Antwort vom ESP32.");

                    port.Close();
                }
            }
        }
    }
}
