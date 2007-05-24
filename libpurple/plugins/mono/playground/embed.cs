using System;
using System.Runtime.InteropServices;

class Util
{
	static public string build_string(IntPtr ptr)
	{
		int len = (int) (uint)strlen(ptr);
		byte[] bytes = new byte [len];
		Marshal.Copy (ptr, bytes, 0, len);
		return System.Text.Encoding.UTF8.GetString (bytes);
	}
	
	[DllImport("libc-2.5")]
	static extern int strlen(IntPtr s);
}

class Embed
{
	static void Main()
	{
		string str = Util.build_string(say_hello());
		set_hello(str);
		
		IntPtr obj = create_struct(10);
		print_struct_value(obj);
	}
		
	[DllImport("./libtest")]
	static extern private IntPtr say_hello();
	
	[DllImport("./libtest")]
	static extern void set_hello(string s);
		
	[DllImport("./libtest")]
	static extern IntPtr create_struct(int v);
	
	[DllImport("./libtest")]
	static extern void print_struct_value(IntPtr s);
}
