using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Purple
{
	public class Signal
	{
		public delegate void Handler(IntPtr p);

		public delegate void VOID__POINTER_POINTER_POINTER_dl(IntPtr p1, IntPtr p2, IntPtr p3, IntPtr signal);

		public static void VOID__POINTER_POINTER_POINTER(IntPtr p1, IntPtr p2, IntPtr p3, IntPtr signal)
		{
			Console.WriteLine("In signal callback: " + p1 + ", " + p2 + ", " + p3 + ", " + signal);
		}

		public static int connect(IntPtr instance, string signal, IntPtr handle, Delegate func, IntPtr data)
		{
			MethodInfo mi = func.GetType().GetMethod("Invoke", BindingFlags.Public | BindingFlags.Instance);
			
			foreach (ParameterInfo p in mi.GetParameters())
			{
				Console.WriteLine(p.ParameterType);
			}

			return _connect(instance, signal, handle, new VOID__POINTER_POINTER_POINTER_dl(VOID__POINTER_POINTER_POINTER), signal);
		}

		private static int _connect(IntPtr instance, string signal, IntPtr handle, Delegate func, string data)
		{
			return purple_signal_connect(instance, signal, handle, func, data);
		}

		[DllImport("libpurple")]
		extern private static int purple_signal_connect(IntPtr instance, string signal, IntPtr handle, Delegate func, string data);
	}
}
