using System;
using System.Net.Sockets;

namespace PVKK_Simulator.Classes
{
    internal class TCP_Connector
    {
        public static void SendData(byte[] data, Action<byte[]> onDataReceived = null)
        {
            // Ziel konfigurieren
            string ipAddress = "192.168.2.193"; // <- HIER Ziel-IP eintragen
            int port = 5000;                  // <- HIER Ziel-Port eintragen

            try
            {
                using (var client = new TcpClient())
                { 
                    client.Connect(ipAddress, port);

                    using (NetworkStream stream = client.GetStream())
                    {
                        stream.Write(data, 0, data.Length);
                        stream.Flush();

                        //Console.WriteLine($"Gesendet: {data.Length} Bytes an {ipAddress}:{port}");

                        if (onDataReceived != null)
                        {
                            byte[] buffer = new byte[10];
                            int totalRead = 0;

                            while ( totalRead < 10) // stream.DataAvailable &&
                            {
                                int bytesRead = stream.Read(buffer, totalRead, 10 - totalRead);
                                if (bytesRead == 0)
                                {
                                    throw new Exception("Verbindung wurde geschlossen, bevor alle Daten empfangen wurden.");
                                }
                                totalRead += bytesRead;
                            }

                            //Console.WriteLine($"Empfangen: {totalRead} Bytes von {ipAddress}:{port}");
                            onDataReceived(buffer);
                        }
                    }  
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Fehler: " + ex.Message);
            }
        }
    }
}
