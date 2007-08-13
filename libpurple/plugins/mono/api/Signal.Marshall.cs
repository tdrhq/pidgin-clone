
using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Purple
{
        public partial class Signal
	{
		private delegate void purple_marshal_VOID_dl(int key);
		private delegate void purple_marshal_VOID__POINTER_POINTER_dl(IntPtr p1, IntPtr p2, int key);
		private delegate void purple_marshal_VOID__POINTER_dl(IntPtr p1, int key);
		private delegate void purple_marshal_VOID__POINTER_POINTER_POINTER_dl(IntPtr p1, IntPtr p2, IntPtr p3, int key);
		private delegate void purple_marshal_VOID__POINTER_INT_INT_dl(IntPtr p1, int p2, int p3, int key);


		private static bool _initalized = false;

		private static void Initalize()
		{
			if (_initalized)
				return;
			_signal_name_to_key.Add("update-idle", 1);
			_signal_name_to_key.Add("blist-node-aliased", 2);
			_signal_name_to_key.Add("buddy-got-login-time", 3);
			_signal_name_to_key.Add("buddy-added", 4);
			_signal_name_to_key.Add("buddy-signed-off", 5);
			_signal_name_to_key.Add("buddy-signed-on", 6);
			_signal_name_to_key.Add("buddy-removed", 7);
			_signal_name_to_key.Add("buddy-status-changed", 8);
			_signal_name_to_key.Add("buddy-privacy-changed", 9);
			_signal_name_to_key.Add("buddy-idle-changed", 10);
			_signal_name_to_key.Add("buddy-icon-changed", 11);
			_signal_name_to_key.Add("blist-node-extended-menu", 12);


			_key_to_delegate.Add(_signal_name_to_key["update-idle"],
				new purple_marshal_VOID_dl(delegate(int key) {
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates);
				}));

			_key_to_delegate.Add(_signal_name_to_key["blist-node-aliased"],
				new purple_marshal_VOID__POINTER_POINTER_dl(delegate(IntPtr p1, IntPtr p2, int key) {
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates, p1, p2);
				}));

			_key_to_delegate.Add(_signal_name_to_key["buddy-got-login-time"],
				new purple_marshal_VOID__POINTER_dl(delegate(IntPtr p1, int key) {
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates, p1);
				}));

			_key_to_delegate.Add(_signal_name_to_key["buddy-status-changed"],
				new purple_marshal_VOID__POINTER_POINTER_POINTER_dl(delegate(IntPtr p1, IntPtr p2, IntPtr p3, int key) {
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates, p1, p2, p3);
				}));

			_key_to_delegate.Add(_signal_name_to_key["buddy-idle-changed"],
				new purple_marshal_VOID__POINTER_INT_INT_dl(delegate(IntPtr p1, int p2, int p3, int key) {
					HandleCallback(_signal_data[key].Args, _signal_data[key].Delegates, p1, p2, p3);
				}));



			_initalized = true;
		}
	}
}
	