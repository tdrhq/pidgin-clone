using System;
using System.Runtime.InteropServices;

namespace Purple {
	public class Buddy : BlistNode {
		private string name;
		private string alias;
		
		public Buddy(IntPtr handle)
			: base(handle)
		{
			Debug.debug(Debug.INFO, "mono", "in Buddy constructor\n");
			IntPtr pname = purple_buddy_get_name(handle);
			Debug.debug(Debug.INFO, "mono", "got buddy name in pointer\n");
			IntPtr palias = purple_buddy_get_alias(handle);
			Debug.debug(Debug.INFO, "mono", "got buddy alias in pointer\n");
			name = Util.build_string(pname);
			Debug.debug(Debug.INFO, "mono", "name is set\n");
			alias = Util.build_string(palias);
			Debug.debug(Debug.INFO, "mono", "alias is set\n");
			
			System.Console.WriteLine(name + " " + alias);
		}

		public Buddy(Account a, string name, string alias)
		{
			this.name = name;
			this.alias = alias;
			_c_handle = purple_buddy_new(a.Handle, name, alias);
		}

		public string Name { get { return name; } }
		public string Alias { get { return alias; } }
		
		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_new(IntPtr h, string name, string alias);
		
		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_get_name(IntPtr h);
		
		[DllImport("libpurple")]
		static private extern IntPtr purple_buddy_get_alias(IntPtr h);

	}
}
