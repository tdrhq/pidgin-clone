use Gaim;

%PLUGIN_INFO = (
	perl_api_version => 2,
	name             => "Example",
	version          => "1.1",
	summary          => "An example Gaim Perl plugin that does nothing useful.",
	description      => "An example Gaim perl script that does nothing particularly useful:\n\t-Show a dialog on load (doesn't currently do this)\n\t-Set user idle for 6,000 seconds (also not currently possible in Perl API)\n\t-Greets people signing on with \"Hello\"\n\t-Informs you when script has been loaded for one minute. (currently in debug output)",
	author           => "Eric Warmenhoven <eric\@warmenhoven.org>",
	url              => "http://gaim.sf.net/",
	load             => "plugin_load",
	unload           => "plugin_unload"
);

sub plugin_init {
	return %PLUGIN_INFO;
}

sub plugin_load {
	Gaim::debug_info("example perl plugin", "plugin_load\n");
	my $plugin = shift;

	# If the notification API had been implemented for Perl, it would be something like this to create a dialog
	# Gaim::notify_info($plugin, "Example Perl Plugin", "Loaded");

	# If there was a way to set the user Idle, it would be done here

	# Listen for when buddies sign on so we can greet them
	Gaim::signal_connect(Gaim::BuddyList::handle, "buddy-signed-on",
		$plugin, \&buddy_signed_on_cb, "Hello!");

	# Add a timer to prompt the user that the plugin has ben loaded for 60 seconds
	Gaim::timeout_add($plugin, 60, \&notify);
}

sub plugin_unload {
	my $plugin = shift;
}

sub buddy_signed_on_cb {
	Gaim::debug_info("example perl plugin", "In buddy_signed_on_cb\n");

	my ($buddy, $data) = @_;

	Gaim::debug_info("example perl plugin", "Buddy '" .
	                 $buddy->get_alias() . "' signed on.\n");

	my $conv = Gaim::Conversation::IM::new($buddy->get_account(), $buddy->get_name());

	$conv->send($data);
}

sub notify {
	Gaim::debug_info("example perl plugin", "gaim example perl plugin has been loaded for 1 minute\n");
}

