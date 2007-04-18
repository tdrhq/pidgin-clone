using System;

namespace Purple {
	public class BlistNode : PObject {
		
		public BlistNode() { }
		public BlistNode(IntPtr handle)
			: base(handle)
		{
			Debug.debug(Debug.INFO, "mono", "in BlistNode constructor\n");
		}
	}
}
