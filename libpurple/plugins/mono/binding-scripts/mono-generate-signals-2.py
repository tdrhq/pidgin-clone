import sys
import re
import string

input = iter(sys.stdin)

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


func_parse = re.compile("purple_signal_register\(handle\s*,\s*\"([A-Za-z0-9_-]+)\"\s*,\s*([A-Za-z0-9_-]+)\s*,\s*[A-Za-z0-9_-]+\s*,\s*([0-9]+)\s*,?\s*(.*)\s*(\);)$");

for func in signal_regs:
	print "func: " + func

	m = func_parse.match(func)

	if m:
		signal_name = m.group(1)
		marshall = m.group(2)
		number_args = int(m.group(3))
		print "signal name: " + signal_name
		print "marshall: " + marshall
		print "number: " + str(number_args)
		if number_args > 0:
			values = m.group(4)
			value_parse_str = "purple_value_new\((PURPLE_TYPE_SUBTYPE)?,?([A-Z_]+)\)\s*,?\s*" * number_args

			value_parse = re.compile(value_parse_str)

			v = value_parse.match(values)

			if v:
				for g in v.groups():
					print g
			print value_parse_str


	print ""

