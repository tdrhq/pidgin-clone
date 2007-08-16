"""
This script looks at a given .h file, and finds each struct Purple* definition.

That Purple struct becomes a seperate class file in api/.  The script will then search
for any functions that operate on that struct and add them as DllImports.

example:

struct _PurpleBuddy { /* blahblahblah */ }
...
char* purple_buddy_get_alias(PurpleBuddy*);

becomes:

class Buddy : Object {
	
	[DllImport("libpurple")]
	IntPtr purple_buddy_get_alias(IntPtr b);
}

The script will have to handle:
* Translation of C types to C# types (any pointer -> IntPtr)
* Handle subclassing (i.e. Buddy should subclass from BListNode)
"""

import sys
import re
import string

def to_function_name(str):
	out = ""
	for c in str:
		if c in string.ascii_uppercase:
			out += "_" + string.lower(c)
		else:
			out += c
	return re.sub("^_", "", out)

def determine_marshall_type(arg):
	"""
	Returns the type used to marshall between C# and C code (i.e. PurpleBuddy* -> IntPtr)
	"""
	s = arg.strip()
	s = re.sub("const", "", s).strip()
	a = s.split()
	if len(a) > 1:
		if a[1].count('*') == 1:
			return "IntPtr"
		return a[0]
	else:
		return a[0]

def determine_csharp_type(arg):
	"""
	Returns the type used by the C# Purple API (i.e. PurpleBuddy* -> Buddy)
	"""
	s = arg.strip()
	s = re.sub("const", "", s).strip()
	a = s.split()
	if re.match("^Purple", a[0]):
		return re.sub("^Purple", "", a[0])
	if re.match("char", a[0]) and a[1].count('*') == 1:
		return "string"
	return a[0]

class Method:
	def __init__(self, ret_str, func_str, arg_str_list):
		self.ret_str = ret_str
		self.func_str = func_str
		self.arg_str_list = arg_str_list

		self.ret_marshall_type = determine_marshall_type(self.ret_str)
		self.ret_csharp_type = determine_csharp_type(self.ret_str)

		self.arg_marshall_type_list = []
		self.arg_csharp_type_list = []


		for arg in self.arg_str_list:
			self.arg_marshall_type_list.append(determine_marshall_type(arg))
			self.arg_csharp_type_list.append(determine_csharp_type(arg))

	
	def as_property(self):
		prop = """
			%s {
				%s;
			}
		"""

		body = ""
		if self.ret_csharp_type == "void":
			body = "Util.build_string("
		else:
			body =  "return "
			body += ""

		#elif self.r

	def as_dllimport(self):
		dllimport = """
		[DllImport("libpurple")]
		static private extern %s %s(%s);
		"""

		return dllimport % (self.ret_marshall_type, self.func_str, string.join(self.arg_marshall_type_list, ","))

	def __repr__(self):
		s =  "Original:  " + self.ret_str + " : " + self.func_str + " : " + str(self.arg_str_list) + "\n"
		s += "Marshall:  " + self.ret_marshall_type + " : " + self.func_str + " : " + str(self.arg_marshall_type_list) + "\n"
		s += "PurpleAPI: " + self.ret_csharp_type + " : " + self.func_str + " : " + str(self.arg_csharp_type_list) + "\n\n"
		return self.as_dllimport() 

class Struct:
	def __init__(self, structname):
		self.structname = structname
		self.csname = re.sub("^Purple", "", structname)
		func_name = to_function_name(structname)
		self.func_nonstatic_parse_str = "([A-Za-z]+\s*\*?)\s*(%s_(get|set|new|on).*)\((.*)\);" % (func_name)
		self.func_parse_str = "([A-Za-z]+\s*\*?)\s*(%s[A-Za-z0-9_]+)\((.*)\);" % (func_name)
		self.func_nonstatic_parse = re.compile(self.func_nonstatic_parse_str)

		self.nonstatic_methods = []

		print self.func_nonstatic_parse_str

	def is_my_func(self, str):
		match = self.func_nonstatic_parse.match(str)

		if not match:
			return False

		ret_type = match.group(1)

		self.nonstatic_methods.append(Method(match.group(1), match.group(2), match.group(4).split(',')))

		return True

	def dump(self):
		for m in self.nonstatic_methods:
			print m
		print ""	

parse_struct = re.compile("struct _(Purple[A-Za-z]+).*")

find_function = re.compile("[A-Za-z]+\s*\*?\s*[A-Za-z0-9_]+\(.*")
func_parse = re.compile("([A-Za-z]+\s*\*?)\s*([A-Za-z0-9_]+)\((.*)\);")

input = iter(sys.stdin)

structs = []

for line in input:
	struct_match = parse_struct.match(line)
	func_find = find_function.match(line)
	if struct_match:
		print struct_match.group(1)
		structs.append(Struct(struct_match.group(1)))
	elif func_find:
		func_line = line.strip()
		while func_line.count('(') > func_line.count(')'):
               		newline = input.next().strip()
	                if len(newline) == 0:
        	            break
                	func_line += newline

		for s in structs:
			if s.is_my_func(func_line):
				break

for s in structs:
	s.dump()
