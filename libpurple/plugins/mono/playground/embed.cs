using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

class Util
{
	static public string build_string(IntPtr ptr)
	{
		int len = (int) (uint)length(ptr);
		byte[] bytes = new byte [len];
		Marshal.Copy (ptr, bytes, 0, len);
		return System.Text.Encoding.UTF8.GetString (bytes);
	}
	
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	private static extern int length(IntPtr s);
}

class Embed
{
	static void Main()
	{
		Dictionary<int, string> dict = new Dictionary<int, string>();
		string str = Util.build_string(say_hello());
		set_hello(str);
		
		IntPtr obj = create_struct("blah face");
		System.Console.WriteLine(Util.build_string(print_struct_value(obj)));
	}
		
	[DllImport("./libtest")]
	static extern private IntPtr say_hello();
	
	[DllImport("./libtest")]
	static extern void set_hello(string s);
		
	[DllImport("./libtest")]
	static extern IntPtr create_struct(string s);
	
	[DllImport("./libtest")]
	static extern IntPtr print_struct_value(IntPtr s);
}
