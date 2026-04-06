<Query Kind="Program">
  <NuGetReference>InputSimulator</NuGetReference>
  <Namespace>WindowsInput</Namespace>
  <Namespace>WindowsInput.Native</Namespace>
  <Namespace>System.Drawing</Namespace>
</Query>

InputSimulator iSim = new InputSimulator();
void Main()
{
	var pos05TextBoxString = new Point(318, 180);
	//Eingabe Wert
	var pos06OeffnenButton = new Point(138, 485);
	var pos07TextBoxDateiName = new Point(1229, 901);
	//Eingabe Dateiname.png
	var pos08OeffnenButton = new Point(1926, 926);
	var pos09GrabGlyphs = new Point(284, 482);

	for (int i = 32; i < 126; i++)
	{
		MoveTo(pos05TextBoxString);
		Thread.Sleep(10);
		Klick();
		Thread.Sleep(10);
		EntferneText();
		Thread.Sleep(10);
		SchreibeText($"{(char)i}");
		Thread.Sleep(10);
		MoveTo(pos06OeffnenButton);
		Thread.Sleep(10);
		Klick();
		Thread.Sleep(10);
		MoveTo(pos07TextBoxDateiName);
		Thread.Sleep(10);
		Klick();
		Thread.Sleep(10);
		for (int a = 0; a < 60; a++) EntferneText();
		Thread.Sleep(10);
		SchreibeText($"{i:000}.png");
		Thread.Sleep(10);
		MoveTo(pos08OeffnenButton);
		Thread.Sleep(10);
		Klick();
		Thread.Sleep(10);
		MoveTo(pos09GrabGlyphs);
		Thread.Sleep(10);
		Klick();
		Thread.Sleep(5000);
	}
}

void SchreibeText(string t)
{
	iSim.Keyboard.TextEntry(t);
}

void EntferneText()
{
	iSim.Keyboard.KeyPress(VirtualKeyCode.BACK);
}

void Klick()
{
	iSim.Mouse.LeftButtonClick();
}

void MoveTo(Point p)
{
	var multiplikatorX = 25.61;
	var multiplikatorY = 45.56;
	iSim.Mouse.MoveMouseTo(multiplikatorX * p.X, multiplikatorY * p.Y);

	var pos = System.Windows.Forms.Cursor.Position;
	pos.Dump();
}