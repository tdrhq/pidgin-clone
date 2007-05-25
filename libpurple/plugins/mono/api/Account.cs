using System;
using System.Runtime.InteropServices;


namespace Purple {
	public class Account : PObject {
		private string username;
		private string protocol_id;

		public Account(IntPtr handle)
			: base(handle)
		{
		}
		
		public Account(string username, string protocol_id)
		{
			this.username = username;
			this.protocol_id = protocol_id;
			_c_handle = purple_account_new(username, protocol_id);
		}

		public string Username { get { return username; } }
		public string ProtocolId { get { return protocol_id; } }

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_get_username(IntPtr handle);

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_get_protocol_id(IntPtr handle);

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_new(string username, string protocol_id);

	}
}
