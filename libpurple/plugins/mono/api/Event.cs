using System;

namespace Purple
{
	public class Event
	{
		private IntPtr handle;
		private string signal;
		
		public Event(IntPtr h, string s)
		{
			handle = h;
			signal = s;
		}
	
		public void connect(IntPtr plugin, Delegate handler)
		{
			Signal.connect(handle, signal, plugin, handler, IntPtr.Zero);
		}
	}
}
