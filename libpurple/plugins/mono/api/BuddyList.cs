using System;
using System.Runtime.InteropServices;

namespace Purple {
	public partial class BuddyList {
	
		public static IntPtr GetHandle()
		{
			return purple_blist_get_handle();
		}

		[DllImport("libpurple")]
		private static extern IntPtr purple_blist_get_handle();
	}
}
