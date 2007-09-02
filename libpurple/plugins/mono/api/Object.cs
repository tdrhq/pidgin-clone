using System;
using System.Runtime.CompilerServices;

namespace Purple {
	public class Object {
		protected IntPtr _c_handle;

		public Object() { }
		public Object(IntPtr h)
		{
			_c_handle = h;
			Debug.debug(Debug.INFO, "mono", "in PObject constructor " + _c_handle + "\n");
		}

		public IntPtr Handle { get { return _c_handle; } }
		
	}
}
