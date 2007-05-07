using System;
using System.Runtime.InteropServices;

class Embed
{
	static void Main()
	{
		string str = build_string(say_hello());
		Console.WriteLine(str);
		set_hello(str);
	}
	
	static string build_string(IntPtr ptr)
	{
		int len = (int) (uint)length(ptr);
		byte[] bytes = new byte [len];
		Marshal.Copy (ptr, bytes, 0, len);
		return System.Text.Encoding.UTF8.GetString (bytes);
	}
	
	[DllImport("./libtest")]
	static extern private IntPtr say_hello();
	
	[DllImport("./libtest")]
	static extern void set_hello(string s);
	
	[DllImport("./libtest")]
	static extern int length(IntPtr s);
}
