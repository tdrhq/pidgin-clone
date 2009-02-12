if ENABLE_CRASHREPORTER

bin_PROGRAMS = crashreporter

crashreporter_SOURCES = \
	crashreporter.c \
	minidump_stackwalk.cc

crashreporter_LDADD = libbreakpad.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/pidgin/crash/google-breakpad


## Libraries
lib_LTLIBRARIES = libbreakpad.la

libbreakpad_la_SOURCES = \
	google-breakpad/google_breakpad/common/breakpad_types.h \
	google-breakpad/google_breakpad/common/minidump_format.h \
	google-breakpad/google_breakpad/common/minidump_size.h \
	google-breakpad/google_breakpad/processor/basic_source_line_resolver.h \
	google-breakpad/google_breakpad/processor/call_stack.h \
	google-breakpad/google_breakpad/processor/code_module.h \
	google-breakpad/google_breakpad/processor/code_modules.h \
	google-breakpad/google_breakpad/processor/memory_region.h \
	google-breakpad/google_breakpad/processor/minidump.h \
	google-breakpad/google_breakpad/processor/minidump_processor.h \
	google-breakpad/google_breakpad/processor/process_state.h \
	google-breakpad/google_breakpad/processor/source_line_resolver_interface.h \
	google-breakpad/google_breakpad/processor/stack_frame.h \
	google-breakpad/google_breakpad/processor/stack_frame_cpu.h \
	google-breakpad/google_breakpad/processor/stackwalker.h \
	google-breakpad/google_breakpad/processor/symbol_supplier.h \
	google-breakpad/google_breakpad/processor/system_info.h \
	google-breakpad/processor/address_map-inl.h \
	google-breakpad/processor/address_map.h \
	google-breakpad/processor/basic_code_module.h \
	google-breakpad/processor/basic_code_modules.cc \
	google-breakpad/processor/basic_code_modules.h \
	google-breakpad/processor/basic_source_line_resolver.cc \
	google-breakpad/processor/call_stack.cc \
	google-breakpad/processor/contained_range_map-inl.h \
	google-breakpad/processor/contained_range_map.h \
	google-breakpad/processor/linked_ptr.h \
	google-breakpad/processor/logging.h \
	google-breakpad/processor/logging.cc \
	google-breakpad/processor/minidump.cc \
	google-breakpad/processor/minidump_processor.cc \
	google-breakpad/processor/pathname_stripper.cc \
	google-breakpad/processor/pathname_stripper.h \
	google-breakpad/processor/postfix_evaluator-inl.h \
	google-breakpad/processor/postfix_evaluator.h \
	google-breakpad/processor/process_state.cc \
	google-breakpad/processor/range_map-inl.h \
	google-breakpad/processor/range_map.h \
	google-breakpad/processor/scoped_ptr.h \
	google-breakpad/processor/simple_symbol_supplier.cc \
	google-breakpad/processor/simple_symbol_supplier.h \
	google-breakpad/processor/stack_frame_info.h \
	google-breakpad/processor/stackwalker.cc \
	google-breakpad/processor/stackwalker_amd64.cc \
	google-breakpad/processor/stackwalker_amd64.h \
	google-breakpad/processor/stackwalker_ppc.cc \
	google-breakpad/processor/stackwalker_ppc.h \
	google-breakpad/processor/stackwalker_sparc.cc \
	google-breakpad/processor/stackwalker_sparc.h \
	google-breakpad/processor/stackwalker_x86.cc \
	google-breakpad/processor/stackwalker_x86.h

endif
