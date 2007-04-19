using System;
using System.Runtime.CompilerServices;


namespace Purple {
	public class Buddy : BlistNode {
		private string name;
		private string alias;
		
		public Buddy(IntPtr handle)
			: base(handle)
		{
			Debug.debug(Debug.INFO, "mono", "in Buddy constructor\n");
		}

		public Buddy(Account a, string name, string alias)
		{
			this.name = name;
			this.alias = alias;
			_c_handle = _purple_buddy_new(a.Handle, name, alias);
		}

		public string Name { get { return name; } }
		public string Alias { get { return alias; } }

		protected override void _updateFromStruct()
		{
			Debug.debug(Debug.INFO, "mono", "updateFromStruct " + _c_handle + " : " + _c_handle.GetType() + "\n");
			name = _purple_buddy_get_name(_c_handle);
			alias = _purple_buddy_get_alias(_c_handle);
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private string _purple_buddy_get_name(IntPtr handle);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private string _purple_buddy_get_alias(IntPtr handle);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private IntPtr _purple_buddy_new(IntPtr account, string name, string alias);

	}
}
