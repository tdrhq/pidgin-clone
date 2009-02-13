
#include <glib.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "google_breakpad/processor/basic_source_line_resolver.h"
#include "google_breakpad/processor/call_stack.h"
#include "google_breakpad/processor/code_module.h"
#include "google_breakpad/processor/code_modules.h"
#include "google_breakpad/processor/minidump.h"
#include "google_breakpad/processor/minidump_processor.h"
#include "google_breakpad/processor/process_state.h"
#include "google_breakpad/processor/stack_frame_cpu.h"
#include "processor/logging.h"
#include "processor/pathname_stripper.h"
#include "processor/scoped_ptr.h"
#include "processor/simple_symbol_supplier.h"

namespace {

using std::string;
using std::vector;
using google_breakpad::BasicSourceLineResolver;
using google_breakpad::CallStack;
using google_breakpad::CodeModule;
using google_breakpad::CodeModules;
using google_breakpad::MinidumpModule;
using google_breakpad::MinidumpProcessor;
using google_breakpad::PathnameStripper;
using google_breakpad::ProcessState;
using google_breakpad::scoped_ptr;
using google_breakpad::SimpleSymbolSupplier;
using google_breakpad::StackFrame;

static GList *GetCrashModules(const string &minidump_file) {
	scoped_ptr<SimpleSymbolSupplier> symbol_supplier;
	std::vector<std::string> symbol_paths;
	symbol_supplier.reset(new SimpleSymbolSupplier(symbol_paths));

	BasicSourceLineResolver resolver;
	MinidumpProcessor minidump_processor(symbol_supplier.get(), &resolver);

	// Process the minidump.
	ProcessState process_state;
	if (minidump_processor.Process(minidump_file, &process_state) !=
		MinidumpProcessor::PROCESS_OK) {
		BPLOG(ERROR) << "MinidumpProcessor::Process failed";
		return NULL;
	}

	int requesting_thread = process_state.requesting_thread();
	if (requesting_thread == -1) {
		return NULL;
	}

	CallStack *stack = process_state.threads()->at(requesting_thread);
	GList *module_names = NULL;
	int frame_count = stack->frames()->size();
	for (int frame_index = 0; frame_index < frame_count; ++frame_index) {
		const StackFrame *frame = stack->frames()->at(frame_index);
		if (frame->module) {
			const char *module_name = PathnameStripper::File(
				frame->module->code_file()).c_str();
			module_names = g_list_append(
				module_names, g_strdup(module_name));
		}
	}

	return module_names;
}

}  // namespace

extern "C" GList *pidgin_crash_module_stack(const char *);

GList* pidgin_crash_module_stack(const char *minidump_file) {
   return GetCrashModules(minidump_file);
}
