using System;
using System.Runtime.InteropServices;

namespace Purple
{
	public class Debug
	{
		public static int ALL = 0;
		public static int MISC = 1;
		public static int INFO = 2;
		public static int WARNING = 3;
		public static int ERROR = 4;
		public static int FATAL = 5;
		
		[DllImport("libpurple")]
		extern private static void purple_debug(int type, string cat, string str);
		
		public static void debug(int type, string cat, string format)
		{
			purple_debug(type, cat, format);
		}
		
		public static void debug(int type, string cat, string format, params object[] args)
		{
			purple_debug(type, cat, String.Format(format, args));
		}
	}
}
