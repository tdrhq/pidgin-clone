using System;
using System.Runtime.InteropServices;

class Embed
{
	static void Main()
	{
		Console.WriteLine(say_hello());
	}
	
	[DllImport("./libtest")]
	static extern private string say_hello();
}
