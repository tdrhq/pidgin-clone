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

class Struct:
	def __init__(self, structname):
		self.structname = structname
		self.csname = re.sub("^Purple", "", structname)
		func_name = to_function_name(structname)
		self.func_parse_str = "([A-Za-z]+\s*\*?)\s*(%s[A-Za-z0-9_]+)\((.*)\);" % (func_name)
		self.func_parse = re.compile(self.func_parse_str)

		self.methods = []

		print self.func_parse_str

	def is_my_func(self, str):
		match = self.func_parse.match(str)

		if not match:
			return False

		self.methods.append(match.group(2))

		return True

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
		"""
		func_match = func_parse.match(func_line)

		if not func_match:
			continue
		
		print func_match.group(1) + " : " + func_match.group(2) + " : " + func_match.group(3) + "\n"
		"""

for s in structs:
	print s.csname, s.methods
