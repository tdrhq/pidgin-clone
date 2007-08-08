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
	

def write_marshalls(signals, out):
	"""
	signals: dict in the form of "signal-name" -> [return type, arg types]
	out: output to write to
	"""



input = iter(sys.stdin)

write_events_and_delegates(parse_signal_register_details(parse_signal_register_calls(input)).items(), sys.argv[1], sys.stdout)

