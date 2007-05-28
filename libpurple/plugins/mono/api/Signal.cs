using System;
using System.Runtime.InteropServices;

namespace Purple
{
	public class Signal
	{
		public delegate void Handler(IntPtr p);

		public delegate void VOID__POINTER_POINTER_POINTER(IntPtr p1, IntPtr p2, IntPtr p3);

		public static int connect(IntPtr instance, string signal, IntPtr handle, Delegate func, IntPtr data)
		{
			return purple_signal_connect(instance, signal, handle, func, data);
		}

		[DllImport("libpurple")]
		extern private static int purple_signal_connect(IntPtr instance, string signal, IntPtr handle, Delegate func, IntPtr data);
	}
}
