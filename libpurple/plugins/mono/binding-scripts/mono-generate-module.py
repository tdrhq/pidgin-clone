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
