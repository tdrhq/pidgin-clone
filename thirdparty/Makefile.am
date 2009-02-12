
if ENABLE_CRASHREPORTER

# This allows #includes to be relative to src/
AM_CPPFLAGS = -I$(top_srcdir)/thirdparty/google-breakpad/src

## Libraries
lib_LTLIBRARIES = google-breakpad/src/libbreakpad.la google-breakpad/src/libbreakpad_client.la

google_breakpad_src_libbreakpad_la_SOURCES = \
	google-breakpad/src/google_breakpad/common/breakpad_types.h \
	google-breakpad/src/google_breakpad/common/minidump_format.h \
	google-breakpad/src/google_breakpad/common/minidump_size.h \
	google-breakpad/src/google_breakpad/processor/basic_source_line_resolver.h \
	google-breakpad/src/google_breakpad/processor/call_stack.h \
	google-breakpad/src/google_breakpad/processor/code_module.h \
	google-breakpad/src/google_breakpad/processor/code_modules.h \
	google-breakpad/src/google_breakpad/processor/memory_region.h \
	google-breakpad/src/google_breakpad/processor/minidump.h \
	google-breakpad/src/google_breakpad/processor/minidump_processor.h \
	google-breakpad/src/google_breakpad/processor/process_state.h \
	google-breakpad/src/google_breakpad/processor/source_line_resolver_interface.h \
	google-breakpad/src/google_breakpad/processor/stack_frame.h \
	google-breakpad/src/google_breakpad/processor/stack_frame_cpu.h \
	google-breakpad/src/google_breakpad/processor/stackwalker.h \
	google-breakpad/src/google_breakpad/processor/symbol_supplier.h \
	google-breakpad/src/google_breakpad/processor/system_info.h \
	google-breakpad/src/processor/address_map-inl.h \
	google-breakpad/src/processor/address_map.h \
	google-breakpad/src/processor/basic_code_module.h \
	google-breakpad/src/processor/basic_code_modules.cc \
	google-breakpad/src/processor/basic_code_modules.h \
	google-breakpad/src/processor/basic_source_line_resolver.cc \
	google-breakpad/src/processor/call_stack.cc \
	google-breakpad/src/processor/contained_range_map-inl.h \
	google-breakpad/src/processor/contained_range_map.h \
	google-breakpad/src/processor/linked_ptr.h \
	google-breakpad/src/processor/logging.h \
	google-breakpad/src/processor/logging.cc \
	google-breakpad/src/processor/minidump.cc \
	google-breakpad/src/processor/minidump_processor.cc \
	google-breakpad/src/processor/pathname_stripper.cc \
	google-breakpad/src/processor/pathname_stripper.h \
	google-breakpad/src/processor/postfix_evaluator-inl.h \
	google-breakpad/src/processor/postfix_evaluator.h \
	google-breakpad/src/processor/process_state.cc \
	google-breakpad/src/processor/range_map-inl.h \
	google-breakpad/src/processor/range_map.h \
	google-breakpad/src/processor/scoped_ptr.h \
	google-breakpad/src/processor/simple_symbol_supplier.cc \
	google-breakpad/src/processor/simple_symbol_supplier.h \
	google-breakpad/src/processor/stack_frame_info.h \
	google-breakpad/src/processor/stackwalker.cc \
	google-breakpad/src/processor/stackwalker_amd64.cc \
	google-breakpad/src/processor/stackwalker_amd64.h \
	google-breakpad/src/processor/stackwalker_ppc.cc \
	google-breakpad/src/processor/stackwalker_ppc.h \
	google-breakpad/src/processor/stackwalker_sparc.cc \
	google-breakpad/src/processor/stackwalker_sparc.h \
	google-breakpad/src/processor/stackwalker_x86.cc \
	google-breakpad/src/processor/stackwalker_x86.h

google_breakpad_src_libbreakpad_client_la_SOURCES = \
	google-breakpad/src/common/convert_UTF.c \
	google-breakpad/src/common/string_conversion.cc \
	google-breakpad/src/common/linux/dump_symbols.cc \
	google-breakpad/src/common/linux/file_id.cc \
	google-breakpad/src/common/md5.c \
	google-breakpad/src/common/linux/guid_creator.cc \
	google-breakpad/src/client/minidump_file_writer.cc \
	google-breakpad/src/client/linux/handler/exception_handler.cc \
	google-breakpad/src/client/linux/handler/minidump_generator.cc \
	google-breakpad/src/client/linux/handler/linux_thread.cc

endif

