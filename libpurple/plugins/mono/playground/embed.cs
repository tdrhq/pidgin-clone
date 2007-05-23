using System;
using System.Runtime.InteropServices;

class Embed
{
	static void Main()
	{
		string str = build_string(say_hello());
		set_hello(str);
		
		IntPtr obj = create_struct(10);
		print_struct_value(obj);
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
	
	[DllImport("./libtest")]
	static extern IntPtr create_struct(int v);
	
	[DllImport("./libtest")]
	static extern void print_struct_value(IntPtr s);
}
