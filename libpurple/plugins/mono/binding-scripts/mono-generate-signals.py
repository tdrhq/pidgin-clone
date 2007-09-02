import sys
import re
import string


def convertname(name):
	newname = ""
	for w in name.split("-"):
		newname += w.capitalize()
	return newname

def parse_signal_register_calls(input):
	"""
	Greps input for purpe_signal_register calls, and collapses calls into a single string
	and returns them in a list
	"""
	signal_reg = re.compile(".*purple_signal_register.*")
	end_call = re.compile(".*\);")

	buffer = ""
	parse_signal_reg = False

	signal_regs = []

	for line in input:
		if not parse_signal_reg:
			if signal_reg.match(line):
				buffer = line.strip()
				if end_call.match(line):
					signal_regs.append(buffer)
					parse_signal_reg = False
					continue
				parse_signal_reg = True
				continue

		if parse_signal_reg:
			buffer = buffer + line.strip()
			if end_call.match(line):
				signal_regs.append(buffer)
				parse_signal_reg = False
				continue
	
	return signal_regs


def parse_signal_register_details(signal_regs):
	"""
	Parses a group of "purple_signal_register" calls into a dict of the form "signal-name" -> [return_type, args]
	"""
	func_parse = re.compile("purple_signal_register\(handle\s*,\s*\"([A-Za-z0-9_-]+)\"\s*,\s*([A-Za-z0-9_-]+)\s*,\s*(.+)\s*,\s*([0-9]+)\s*,?\s*(.*)\s*(\);)$");

	return_type_parse = re.compile("(purple_value_new\(PURPLE_TYPE_SUBTYPE,|purple_value_new\()?\s*([A-Za-z_-]+)")

	signals = {}

	for func in signal_regs:
		m = func_parse.match(func)
	
		if m:
			signal_name = m.group(1)
			marshall = m.group(2)
			return_type_str = m.group(3)
			number_args = int(m.group(4))

			return_type_m = return_type_parse.match(return_type_str)

			signals[signal_name] = {"types": [return_type_m.group(2)], "marshall": marshall}

			#print "Parsing signal: " + signal_name

			if number_args > 0:
				values = m.group(5)
	
				value_parse = re.compile("purple_value_new\((PURPLE_TYPE_SUBTYPE|PURPLE_TYPE_BOXED)?,?\s*(.+)\)\s*,?\s*" * number_args)

				v = value_parse.match(values)

				if v:
					for g in v.groups():
						if g and g != "PURPLE_TYPE_SUBTYPE" and g != "PURPLE_TYPE_BOXED":
							signals[signal_name]["types"].append(g)

	return signals


	


from monotypes import *

def write_events_and_delegates(signals, classname, out):
	"""
	Creates a C# class with Events, and delegate handles

	signals: dictionary in the form of "signal-name" -> [return type, types]"
	classname: desired C# classname
	out: output to write to
	"""

	head = """
namespace Purple {
	public partial class %s {
"""

	out.write(head % (classname))

	for k, d in signals:
		event_name = "On" + convertname(k)
		delegate_name = convertname(k) + "Handle";

		output = "\t\tpublic static Event " + event_name + " = new Event(GetHandle(), \"" + k + "\");\n";

		output += "\t\tpublic delegate "

		v = d["types"]

		ret_type = v[0]

		output += types[ret_type] + " " + delegate_name + "("

		if v[1:] == []:
			output += ");"
		else:
			i = 1
			for t in v[1:]:
				try:
					output += types[t] + " arg" + str(i) + ", "
					i += 1
				except KeyError:
					output = None
					break

		if output:
			output = re.sub(", $", ");", output)
			out.write(output + "\n\n")

	out.write("\t}\n}")
	
class InvalidType:
	pass

def create_marshall_args_str(argtypes):
	is_intptr = re.compile("(PURPLE_SUBTYPE_.*|PURPLE_TYPE_STRING)")

	arg_str = ""
	i = 1

	for arg in argtypes[1:]:
		if is_intptr.match(arg):
			arg_str += "IntPtr p%d, " % (i)
			i += 1
		else:
			try:
				arg_str += "%s p%d, " % (types[arg], i)
				i += 1
			except KeyError:
				raise InvalidType	

	arg_str += "int key"

	return arg_str 

def write_marshalls(signals, out):
	"""
	signals: dict in the form of "signal-name" -> [return type, arg types]
	out: output to write to
	"""

	marshalls = {}
	added_marshalls = {}
	marshalls_out = ""

	name_to_key = ""
	key_to_delegate = ""

	for sig_name, sig_info in signals:
		argtypes = sig_info["types"]
		marshall = sig_info["marshall"]

		
		# figure out marshall args and types
		try:
			m = "%s_dl" % (marshall)
			arg_str = create_marshall_args_str(argtypes)
			marshalls[sig_name] = {"m": m, "arg_str": arg_str, "arg_len": len(argtypes) - 1}
			added_marshalls[marshall] = True
			if not added_marshalls.has_key(marshall):
				marshalls_out += "\t\tprivate delegate %s %s(%s);\n" % (types[argtypes[0]], m, arg_str)
		except InvalidType:
			pass
		
	i = 1
	for sig_name, sig_info in signals:
		name_to_key += "\t\t\t_signal_name_to_key.Add(\"%s\", %d);\n" % (sig_name, i)

		t = ""
		try:
			t += "\t\t\t_key_to_delegate.Add(_signal_name_to_key[\"%s\"],\n" % (sig_name)
			t += "\t\t\t\tnew %s(delegate(%s) {\n" % (marshalls[sig_name]["m"], marshalls[sig_name]["arg_str"])
			t += "\t\t\t\t\tHandleCallback(_signal_data[key].Args, _signal_data[key].Delegates,"
			for j in range(marshalls[sig_name]["arg_len"]):
				t += " p%d," % (j + 1)
			t = re.sub(",$", ");", t)
			t += "\n\t\t\t\t}));\n\n"
		except KeyError:
			t = ""
			pass

		key_to_delegate += t
		i += 1

	output = """
using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Purple
{
        public partial class Signal
	{
%s

		private static bool _initalized = false;

		private static void Initalize()
		{
			if (_initalized)
				return;

%s

%s
			_initalized = true;
		}
	}
}
	"""

	out.write(output % (marshalls_out, name_to_key, key_to_delegate))



input = iter(sys.stdin)


if len(sys.argv) < 2:
	print "%s: --events ClassName --marshalls" % (sys.argv[0])
	sys.exit(1)


if sys.argv[1] == "--events":
	classname = sys.argv[2]
	write_events_and_delegates(parse_signal_register_details(parse_signal_register_calls(input)).items(), classname, sys.stdout)

elif sys.argv[1] == "--marshalls":
	write_marshalls(parse_signal_register_details(parse_signal_register_calls(input)).items(), sys.stdout)
