<?
	$page->title = 'Plugins';
	require('inc/template.inc.php');
?>

<h1>Plugins</h1>

<?
	$plugins = array(
/*		"gaim-blogger" => array(
			"URL"  => "http://gaim-blogger.sourceforge.net",
			"TEXT" => "Makes use of Purple's IM interface to post, edit, view and track blogs"
		),
*/
		"<del>Meanwhile Protocol Plugin" => array(
			"URL"  => "http://meanwhile.sourceforge.net/",
			"TEXT" => "The Meanwhile Project is an open implementation of the Lotus Sametime Community Client protocol.</del> <br/>The Meanwhile code has been merged into libpurple/Pidgin."
		),

		"guifications" => array(
			"URL"  => "http://pidgin.guifications.org/trac/wiki/Guifications",
			"TEXT" => "Graphical notification when buddies sign on, sign off, go away, etc."
		),

		"Extended Preferences" => array(
			"URL"  => "http://gaim-extprefs.sourceforge.net/",
			"TEXT" => "The Pidgin Extended Preferences Plugin adds additional preferences that have been commonly called for in the past from Pidgin that are either already implemented and hidden, or trivial to implement via a plugin."
		),

		"IRC Helper" => array(
			"URL" => "http://plugins.guifications.org/trac/wiki/irchelper",
			"TEXT" => "The Pidgin IRC Helper plugin which seeks to handle the rough edges of the IRC protocol through network-specific code.<br/>IRC Helper has moved to the plugin pack."
		),

		"gaim-encryption" => array(
			"URL"  => "http://gaim-encryption.sourceforge.net/",
			"TEXT" => "Another Pidgin encryption plugin (RSA encryption)"
		),

		"Off-the-Record Messaging" => array(
			"URL" => "http://www.cypherpunks.ca/otr/",
			"TEXT" => "Off-the-Record (OTR) Messaging allows you to have private (not just encrypted) conversations over instant messaging by providing:  Encryption, Authentication, Deniability, and Perfect forward secrecy."
		),

		"PyGaim" => array(
			"URL" => "http://pygaim.sourceforge.net/",
			"TEXT" => " PyGaim is a gaim plugin that enables Pidgin to be extensible with Python."
		),

		"Purple Plugin Pack" => array(
			"URL" => "http://purple.guifications.org/trac/wiki/PluginPack",
			"TEXT" => "The Purple Plugin Pack is a collection of over 30 plugins for libpurple and Pidgin, including a few that implement often-requested features for Pidgin."
		),

	);
?>

<p>
 Welcome to our plugins page! Here you'll find a listing of some of the plugins
 available for Pidgin written by Pidgin users. There are other great ones listed in
 the <a href="http://sourceforge.net/tracker/?atid=390395&amp;group_id=235&amp;func=browse">Plugins</a>
 section on our SourceForge project page. Remember, the Protocol Plugins come
 with Pidgin.
</p>

<ul>

<?php
	while (list($name, $data) = each($plugins)) {
		print "  <li>";
		print '<a href="' . $data["URL"] . '">';
		print $name;
		print "</a> - ";
		print $data["TEXT"];
		print "</li>\n";
	}
?>

</ul>

<p>
 More to come later....
</p>

