using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class BuddyList {
		public static Event OnBuddyStatusChanged =
			new Event(GetHandle(), "buddy-status-changed");

		public delegate void BuddyStatusChangedHandle(Buddy p1, Status p2, Status p3);
	
		public static IntPtr GetHandle()
		{
			return purple_blist_get_handle();
		}

		[DllImport("libpurple")]
		private static extern IntPtr purple_blist_get_handle();
	}
}
