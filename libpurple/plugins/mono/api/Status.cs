using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class Status : Object {
		private string id;
		private string name;
		
		public Status(IntPtr handle)
			: base(handle)
		{
			id = Util.build_string(purple_status_get_id(handle));
			name = Util.build_string(purple_status_get_name(handle));
		}

		public string Id { get { return id; } }
		public string Name { get { return name; } }

		[DllImport("libpurple")]
		private static extern IntPtr purple_status_get_id(IntPtr h);

		[DllImport("libpurple")]
		private static extern IntPtr purple_status_get_name(IntPtr h);
	}
}
