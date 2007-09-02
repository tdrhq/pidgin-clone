using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class Status : Object {
		
		public Status(IntPtr handle)
			: base(handle)
		{
		}

		public string Id {
			get {
				return Util.build_string(purple_status_get_id(Handle));
			}
		}

		public string Name {
			get {
				return Util.build_string(purple_status_get_name(Handle));
			}
		}

		[DllImport("libpurple")]
		private static extern IntPtr purple_status_get_id(IntPtr h);

		[DllImport("libpurple")]
		private static extern IntPtr purple_status_get_name(IntPtr h);
	}
}
