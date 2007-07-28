using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Purple
{
	public partial class Signal
	{
		/*
		 * Internal class to map one signal to multiple registered delegates
		 */
		private class SignalData
		{
			private Type _return_type;
			private ParameterInfo[] _argument_types;
			private List<Delegate> _delegates;

			public SignalData(Type ret, ParameterInfo[] args)
			{
				_return_type = ret;
				_argument_types = args;
				_delegates = new List<Delegate>();
			}

			public void AddDelegate(Delegate d)
			{
				_delegates.Add(d);
			}

			public ParameterInfo[] Args { get { return _argument_types; } }
			public List<Delegate> Delegates { get { return _delegates; } }
		}

		// Key -> SignalData
		private static Dictionary<int, SignalData> _signal_data = new Dictionary<int, SignalData>();

		// String signal -> Key
		private static Dictionary<string, int> _signal_name_to_key = new Dictionary<string, int>();
		
		// Key -> Delegate "marshall"
		private static Dictionary<int, Delegate> _key_to_delegate = new Dictionary<int, Delegate>();

		/*
		 * Helper method to iterate over delegates and invoke them
		 */
		private static void DispatchCallbacks(List<Delegate> delegates, object[] args)
		{
			foreach (Delegate d in delegates)
			{
				d.DynamicInvoke(args);
			}
		}

		/*
		 * Helper method for marshalls.  Takes a list of types, delegates, and arguments to the delegates.
		 * If one of the args is a IntPtr then we'll build a Purple.Object out of it, otherwise
		 * we'll just forward it on to the delegates.
		 */
		private static void HandleCallback(ParameterInfo[] arginfo, List<Delegate> delegates, params object[] args)
		{
			List<object> objs = new List<object>();

			for (int i = 0; i < args.Length; i++)
			{
				if (args[i].GetType() == typeof(IntPtr)) {
					objs.Add(ObjectManager.GetObject((IntPtr)args[i], arginfo[i].ParameterType));
				} else {
					objs.Add(args[i]);
				}
			}

			DispatchCallbacks(delegates, objs.ToArray());
		}

		/*
		 * Helper method for marshalls that only have IntPtr args
		 */
		private static void HandleIntPtrCallback(ParameterInfo[] args, List<Delegate> delegates, params IntPtr[] structs)
		{
			List<Object> objs = new List<Object>();

			for (int i = 0; i < structs.Length; i++)
			{
				objs.Add(ObjectManager.GetObject(structs[i], args[i].ParameterType));
			}

			DispatchCallbacks(delegates, objs.ToArray());
		}

		/*
		 * Frontend connect for everyone.  Used by Event.connect
		 */
		public static int connect(IntPtr instance, string signal, IntPtr handle, Delegate func, IntPtr data)
		{
			Initalize();
			MethodInfo mi = func.GetType().GetMethod("Invoke", BindingFlags.Public | BindingFlags.Instance);

			int key = _signal_name_to_key[signal]; 

			if (!_signal_data.ContainsKey(key)) {
				_signal_data.Add(key, new SignalData(null, mi.GetParameters()));
			}

			SignalData sd = _signal_data[key];

			sd.AddDelegate(func);

			return _connect(instance, signal, handle, _key_to_delegate[key], key);
		}

		private static int _connect(IntPtr instance, string signal, IntPtr handle, Delegate func, int data)
		{
			return purple_signal_connect(instance, signal, handle, func, data);
		}

		[DllImport("libpurple")]
		extern private static int purple_signal_connect(IntPtr instance, string signal, IntPtr handle, Delegate func, int data);
	}
}
