using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Purple
{
	public partial class Signal
	{
		/*
		 * Delegate "marshall" definitions
		 */
		private delegate void VOID__POINTER_POINTER_POINTER_dl(IntPtr p1, IntPtr p2, IntPtr p3, int signal);

		/*
		 * C# 2.0 does not support in-place initalization of generics so we use this method
		 */
		private static bool _initalized = false;

		private static void Initalize()
		{
			if (_initalized) 
				return;

			// string signal name to integer key
			_signal_name_to_key.Add("buddy-status-changed", 1);

			// integer key to delegate "marshall"
			_key_to_delegate.Add(_signal_name_to_key["buddy-status-changed"], 
				new VOID__POINTER_POINTER_POINTER_dl(delegate(IntPtr p1, IntPtr p2, IntPtr p3, int key) { 
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates, p1, p2, p3); 
				}));

			_initalized = true;
		}
	}
}
