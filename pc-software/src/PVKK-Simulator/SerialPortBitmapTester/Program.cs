using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace SerialPortBitmapTester
{
    internal class Program
    {
        const int Width = 128;
        const int Height = 64;

        static void Main(string[] args)
        {
            //using (var serial = new SerialPort("COM3", 115200, Parity.None, 8, StopBits.One))
            //{
            //    var sw = new System.Diagnostics.Stopwatch();
            //    sw.Start();
            //    serial.Open();
            //    Console.WriteLine($"Open: {sw.Elapsed}");
            //    using (var bmp = new Bitmap(Width, Height, PixelFormat.Format24bppRgb))
            //    {
            //        //CreateBitmap(i.ToString("00"), bmp);
            //        CreateBitmap("KWG", bmp);
            //        Console.WriteLine($"Create: {sw.Elapsed}");

            //        // 2) In SH1106-Rohbytes umwandeln (1 Bit, Page-Format)
            //        byte[] buffer = ConvertBitmapToSh1106(bmp);
            //        Console.WriteLine($"Convert: {sw.Elapsed}");

            //        // 3) Über Serial senden
            //        //SendDataOverSerial(buffer);
            //        serial.Write(buffer, 0, buffer.Length);
            //        Console.WriteLine($"Write: {sw.Elapsed}");
            //    }

            //    serial.Close();
            //    Console.WriteLine($"Close: {sw.Elapsed}");
            //    sw.Stop();
            //}


            //for (int i = 20; i >= 0; i--)
            //{
            // 1) Bitmap erstellen
            using (var bmp = new Bitmap(Width, Height, PixelFormat.Format24bppRgb))
            {
                //CreateBitmap(i.ToString("00"), bmp);
                CreateBitmap("KWG", bmp);

                // 2) In SH1106-Rohbytes umwandeln (1 Bit, Page-Format)
                byte[] buffer = ConvertBitmapToSh1106(bmp);

                // 3) Über Serial senden
                SendDataOverSerial(buffer);
            }

                Thread.Sleep(10000);
            //}

            Console.ReadLine();
        }

        static byte[] ConvertBitmapToSh1106(Bitmap bmp)
        {
            var divider = 8;
            byte[] data = new byte[Width * Height / divider]; // 1024 Bytes
            int index = 0;

            for (int page = 0; page < Height / divider; page++)
            {
                for (int x = 0; x < Width; x++)
                {
                    byte b = 0;
                    for (int bit = 0; bit < 8; bit++)
                    {
                        int y = page * 8 + bit;
                        Color pixel = bmp.GetPixel(x, y);
                        bool isOn = (pixel.R + pixel.G + pixel.B) > 0;
                        if (isOn)
                            b |= (byte)(1 << bit);
                    }
                    data[index++] = b;
                }
            }

            return data;
        }

        static void SendDataOverSerial(byte[] data)
        {
            using (var serial = new SerialPort("COM3", 115200, Parity.None, 8, StopBits.One))
            {
                serial.Open();
                serial.Write(data, 0, data.Length);
                serial.Close();
            }
        }

        static void CreateBitmap(string Text, Bitmap bmp)
        {
            using (var g = Graphics.FromImage(bmp))
            {
                var format = new StringFormat
                {
                    LineAlignment = StringAlignment.Center,
                    Alignment = StringAlignment.Center
                };
                g.Clear(Color.Black);
                using (var font = new Font("14 SEGMENT DISPLAY", 34, FontStyle.Bold))
                {
                    g.DrawString(Text, font, Brushes.White, new PointF(Width / 2, Height / 2), format);
                }
            }
        }
    }
}
