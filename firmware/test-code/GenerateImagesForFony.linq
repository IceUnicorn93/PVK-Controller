<Query Kind="Statements">
  <Namespace>System.Drawing</Namespace>
</Query>

var Height = 42;
var Width = 27;
var fontSize = 31.5f;

for (int i = 0; i < 255; i++)
{
	try
	{	        
		using (var bmp = new Bitmap(Width, Height))
		{
			using (var g = Graphics.FromImage(bmp))
			{
				g.FillRectangle(new SolidBrush(Color.Black), new Rectangle(0, 0, Width, Height));
				var size = g.MeasureString($"{i}", new System.Drawing.Font("14 SEGMENT DISPLAY", fontSize));
				g.DrawString($"{(char)i}", new System.Drawing.Font("14 SEGMENT DISPLAY", fontSize), new SolidBrush(Color.White), new Point(-7, 0));
			}
			//bmp.Save(@$"YourPathToFony\Fony-wip\Font_Images\{i:000}-{(char)i}.png");
			bmp.Save(@$"YourPathToFony\Fony-wip\Font_Images\{i:000}.png");
		}
	}
	catch {}
}

