using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections;

namespace Purple
{
	public class Signal
	{
		private class SignalData
		{
			private Type _return_type;
			private ParameterInfo[] _argument_types;
			private ArrayList _method_infos;
			private ArrayList _delegates;

			public SignalData(Type ret, ParameterInfo[] args)
			{
				_return_type = ret;
				_argument_types = args;
				_delegates = new ArrayList();
				_method_infos = new ArrayList();
			}

			public void AddDelegate(MethodInfo mi, Delegate d)
			{
				_method_infos.Add(mi);	
				_delegates.Add(d);
			}

			public ParameterInfo[] Args { get { return _argument_types; } }
			public ArrayList MethodInfos { get { return _method_infos; } }
			public ArrayList Delegates { get { return _delegates; } }
		}

		private static Hashtable _signal_data = new Hashtable();

		public delegate void Handler(IntPtr p);

		public delegate void VOID__POINTER_POINTER_POINTER_dl(IntPtr p1, IntPtr p2, IntPtr p3, int signal);

		public static void VOID__POINTER_POINTER_POINTER(IntPtr p1, IntPtr p2, IntPtr p3, int signal)
		{
			Console.WriteLine("In signal callback: " + p1 + ", " + p2 + ", " + p3 + ", " + signal);
			SignalData sd = ((SignalData)_signal_data[signal]);

			ParameterInfo[] p = sd.Args;
			
			Object o1 = ObjectManager.GetObject(p1, p[0].ParameterType);
			Object o2 = ObjectManager.GetObject(p2, p[1].ParameterType);
			Object o3 = ObjectManager.GetObject(p3, p[2].ParameterType);

			int i = 0;
			foreach (Delegate d in sd.Delegates)
			{
				d.DynamicInvoke(new object[] { o1, o2, o3 } );
			}
		}

		public static int connect(IntPtr instance, string signal, IntPtr handle, Delegate func, IntPtr data)
		{
			MethodInfo mi = func.GetType().GetMethod("Invoke", BindingFlags.Public | BindingFlags.Instance);

			int key = 1; // This is hardcoded right now;  In the future this would come from a mapping
				     // of string signal -> ints

			if (!_signal_data.ContainsKey(key)) {
				_signal_data.Add(key, new SignalData(null, mi.GetParameters()));
			}

			SignalData sd = _signal_data[key] as SignalData;

			sd.AddDelegate(mi, func);

			return _connect(instance, signal, handle, new VOID__POINTER_POINTER_POINTER_dl(VOID__POINTER_POINTER_POINTER), key);
		}

		private static int _connect(IntPtr instance, string signal, IntPtr handle, Delegate func, int data)
		{
			return purple_signal_connect(instance, signal, handle, func, data);
		}

		[DllImport("libpurple")]
		extern private static int purple_signal_connect(IntPtr instance, string signal, IntPtr handle, Delegate func, int data);
	}
}
