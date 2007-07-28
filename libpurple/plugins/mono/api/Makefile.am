monodir=$(libdir)/purple-$(PURPLE_MAJOR_VERSION)

mono_sources = \
	Object.cs \
	ObjectManager.cs \
	BlistNode.cs \
	BuddyList.cs \
	Buddy.cs \
	Contact.cs \
	Debug.cs \
	Event.cs \
	PurplePlugin.cs \
	Group.cs \
	Signal.cs \
	Signal.Marshall.cs \
	Status.cs \
	Account.cs \
	Util.cs

EXTRA_DIST = $(mono_sources)

mono_SCRIPTS = PurpleAPI.dll

mono_build_sources = $(addprefix $(srcdir)/, $(mono_sources))

all: $(mono_SCRIPTS)

$(mono_SCRIPTS): $(mono_build_sources)
	gmcs -t:library -out:$(mono_SCRIPTS) $(mono_build_sources)

clean-local:
	rm -rf $(mono_SCRIPTS)
