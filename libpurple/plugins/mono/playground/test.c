#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

char* say_hello();
void set_hello(char *str);

int main(int argc, char *argv[])
{
	MonoDomain *domain;
	MonoAssembly *assm;
	const char *file;
	
	file = argv[1];
	
	// Setup the runtime
	domain = mono_jit_init("test"); 
	
	// Open the assembly
	assm = mono_domain_assembly_open(domain, "embed.exe");
	
	// Run Main in the assembly
	mono_jit_exec (domain, assm, argc, argv);
	
	// Clean everything up
	mono_jit_cleanup (domain);
}
