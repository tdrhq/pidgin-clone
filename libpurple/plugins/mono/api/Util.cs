using System;
using System.Runtime.InteropServices;

namespace Purple {
	class Util {
		static public string build_string(IntPtr ptr)
		{
			if (ptr == IntPtr.Zero)
				return "";

			int len = (int) (uint)ml_strlen(ptr);
			byte[] bytes = new byte [len];
			Marshal.Copy (ptr, bytes, 0, len);
			return System.Text.Encoding.UTF8.GetString (bytes);
		}
		
		[DllImport("libpurple")]
		static extern int ml_strlen(IntPtr s);
	}
}
