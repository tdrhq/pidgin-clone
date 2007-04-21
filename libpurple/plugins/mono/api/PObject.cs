using System;
using System.Runtime.CompilerServices;

namespace Purple {
	public class PObject {
		protected IntPtr _c_handle;

		public PObject() { }
		public PObject(IntPtr h)
		{
			_c_handle = h;
			//_updateFromStruct();
			Debug.debug(Debug.INFO, "mono", "in PObject constructor " + _c_handle + "\n");
		}

		public IntPtr Handle { get { return _c_handle; } }
		
		protected virtual void _updateFromStruct()
		{

		}

	}
}
