using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class Buddy : BlistNode {
		private string name;
		private string alias;
		
		public Buddy(IntPtr handle)
			: base(handle)
		{
			name = Util.build_string(purple_buddy_get_name(handle));
			alias = Util.build_string(purple_buddy_get_alias(handle));
		}

		public Buddy(Account a, string name, string alias)
		{
			this.name = name;
			this.alias = alias;
			_c_handle = purple_buddy_new(a.Handle, name, alias);
		}

		public string Name { get { return name; } } 
		public string Alias { get { return alias; } }
		public Account PAccount { 
			get { 
				Account a = new Account(purple_buddy_get_account(_c_handle));
				return a;
			} 
		}
		
		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_new(IntPtr h, string name, string alias);
		
		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_get_name(IntPtr h);

		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_get_alias(IntPtr h);

		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_get_account(IntPtr h);

	}
}
