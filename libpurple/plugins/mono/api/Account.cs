using System;
using System.Runtime.CompilerServices;


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
			_c_handle = _purple_account_new(username, protocol_id);
		}

		public string Username { get { return username; } }
		public string ProtocolId { get { return protocol_id; } }

		protected override void _updateFromStruct()
		{
			username = _purple_account_get_username(_c_handle);
			protocol_id = _purple_account_get_protocol_id(_c_handle);
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private static string _purple_account_get_username(IntPtr handle);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private static string _purple_account_get_protocol_id(IntPtr handle);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private static IntPtr _purple_account_new(string username, string protocol_id);

	}
}
