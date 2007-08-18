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

TODO: Figure out a clean way to create C# Properties for a given class
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

def convertname(name):
	newname = ""
	for w in name.split("-"):
		newname += w.capitalize()
	return newname

def clean_arg(arg):
	return re.sub("const", "", arg.strip()).strip()

def is_purple_object(arg):
	a = clean_arg(arg).split()
	if len(a) != 2:
		return False
	return (re.match("^Purple", a[0]) and a[1].count('*') == 1)

def determine_marshall_type(arg):
	"""
	Returns the type used to marshall between C# and C code (i.e. PurpleBuddy* -> IntPtr)
	TODO: handle "primitive" types (i.e. gboolean -> bool)
	"""
	a = clean_arg(arg).split()
	if len(a) > 1:
		if a[1].count('*') == 1:
			return "IntPtr"
		return a[0]
	else:
		return a[0]

def determine_csharp_type(arg):
	"""
	Returns the type used by the C# Purple API (i.e. PurpleBuddy* -> Buddy)
	TODO: handle "primitive" types (i.e. gboolean -> bool)
	"""
	a = clean_arg(arg).split()
	if is_purple_object(arg):
		return re.sub("^Purple", "", a[0])
	if re.match("char", a[0]) and a[1].count('*') == 1:
		return "string"
	return a[0]

class ArgType:
	def __init__(self, cstr):
		self.marshall = determine_marshall_type(cstr)
		self.csharp = determine_csharp_type(cstr)
		self.is_purple_object = is_purple_object(cstr) 

class Property:
	def __init__(self, propname):
		self.propname = propname.strip('_')
		self.cspropname = convertname(self.propname)

		self.actions = {}

	def add(self, operation, method):
		self.actions[operation] = method

	def dump(self):
		property = """
		public %s %s {
			%s
		}
		"""

		body = ""
		if self.actions.has_key('get'):
			meth = self.actions['get']
			body += "get { "
			body += "return "


class Method:
	def __init__(self, ret_str, func_str, arg_str_list):
		self.ret_str = ret_str
		self.func_str = func_str
		self.arg_str_list = arg_str_list

		self.ret_type = ArgType(self.ret_str)

		self.arg_type_list = []

		for arg in self.arg_str_list:
			self.arg_type_list.append(ArgType(arg))

	
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
			if self.ret_csharp_type == "string":
				body += "Util.build_string(%s);" % (self.func_str)
			body += ""

		#elif self.r

	def as_dllimport(self):
		dllimport = """
		[DllImport("libpurple")]
		static private extern %s %s(%s);
		"""

		return dllimport % (self.ret_type.marshall, self.func_str, string.join([s.marshall for s in self.arg_type_list], ","))

	def __repr__(self):
		return self.as_dllimport() 

class Struct:
	def __init__(self, structname):
		self.structname = structname
		self.csname = re.sub("^Purple", "", structname)
		func_name = to_function_name(structname)
		self.func_nonstatic_parse_str = "([A-Za-z]+\s*\*?)\s*(%s_(get|set|new|on)(.*))\((.*)\);" % (func_name)
		self.func_parse_str = "([A-Za-z]+\s*\*?)\s*(%s[A-Za-z0-9_]+)\((.*)\);" % (func_name)
		self.func_nonstatic_parse = re.compile(self.func_nonstatic_parse_str)

		self.nonstatic_methods = []

		self.properties = {}

		print self.func_nonstatic_parse_str

	def is_my_func(self, str):
		match = self.func_nonstatic_parse.match(str)

		if not match:
			return False

		ret_type = match.group(1)
		func_name = match.group(2)
		operation = match.group(3)
		prop_name = match.group(4)
		args = match.group(5)

		meth = Method(ret_type, func_name, args.split(','))

		if operation != "new":
			if not self.properties.has_key(prop_name):
				self.properties[prop_name] = Property(prop_name)

			self.properties[prop_name].add(operation, meth)

		self.nonstatic_methods.append(meth)

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
