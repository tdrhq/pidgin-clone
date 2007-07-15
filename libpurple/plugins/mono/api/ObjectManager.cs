using System;
using System.Collections;
using System.Reflection;

namespace Purple {
	class ObjectManager {
		private static Hashtable _objects;

		public static Object GetObject(IntPtr handle, Type t)
		{
			if (handle == IntPtr.Zero)
				return null;

			WeakReference wref = _objects[handle] as WeakReference;
			Object obj = null;

			if (wref == null || !wref.IsAlive) {
				_objects[handle] = null;
				obj = CreateObject(handle, t);
			} else {
				obj = wref.Target as Object;
			}

			return obj;
		}

		private static Object CreateObject(IntPtr handle, Type t)
		{
			ConstructorInfo ctor = t.GetConstructor(BindingFlags.NonPublic | BindingFlags.Instance, null, new Type[] { typeof(IntPtr) }, null);

			if (ctor != null) {
				Object obj = ctor.Invoke(new object[] { handle }) as Object;
				_objects[handle] = new WeakReference(obj);
				return obj;
			}
			return null;
		}
	}
}
