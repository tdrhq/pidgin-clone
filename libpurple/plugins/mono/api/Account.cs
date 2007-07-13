using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class Account : Object {

		public Account(IntPtr handle)
			: base(handle)
		{
		}
		
		public Account(string username, string protocol_id)
		{
			_c_handle = purple_account_new(username, protocol_id);
		}

		public string Username {
			get {
				return Util.build_string(purple_account_get_username(Handle));
			}
		}

		public string ProtocolId {
			get {
				return Util.build_string(purple_account_get_protocol_id(Handle));
			}
		}

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_get_username(IntPtr handle);

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_get_protocol_id(IntPtr handle);

		[DllImport("libpurple")]
		extern private static IntPtr purple_account_new(string username, string protocol_id);

	}
}
