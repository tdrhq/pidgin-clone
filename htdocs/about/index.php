<?
  // Update these to match the current page.
  $page['title'] =   "About Pidgin, the universal IM client";
  $page['section'] = "About";
  $page['description'] = "General information about the Pidgin IM client.";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");

?>

<div id="content">





<div class="box_full">
<div id="main">

<h1>About Pidgin, the universal IM client</h1>

<div style="float:right;">
<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download.inc");
?>
<br/>
<div style="text-align:center">
<img src="/shared/img/contact_window.png" alt="Buddy List" width="192" height="420" />
</div>
</div>

<p>Pidgin is an IM program which lets you log in to multiple accounts on multiple IM networks
simultaneously. This means that you can be chatting with friends on AIM,
talking to a friend on Yahoo Messenger, and sitting in an IRC channel all at
the same time.</p>

<p>Pidgin runs on Windows, Linux, and other UNIX operating systems. Looking for Pidgin for OS X? Try <a
href="http://www.adiumx.com/">Adium</a>!</p>

<p>Pidgin is compatible with the following IM networks: 
AIM, ICQ, Google Talk, Jabber/XMPP, MSN
Messenger, Yahoo!, Bonjour, Gadu-Gadu, IRC, Novell GroupWise Messenger, QQ,
Lotus Sametime, SILC, SIMPLE, MySpaceIM, and Zephyr.</p>

<p>Pidgin supports many features of the various IM networks, such as file
transfer, away messages, and typing notification. It also goes beyond that
and provides many unique features. A few popular features are Buddy Pounces,
which give the ability to notify you, send a message, play a sound, or run a
program when a specific buddy goes away, signs online, or returns from idle;
and plugins, consisting of text replacement, a buddy ticker, extended message
notification, iconify on away, spell checking, tabbed conversations, and more.
</p>

<p>Pidgin integrates well with GNOME 2 and KDE 3.1's system tray, as well as
Windows' own system tray. This allows you to work with Pidgin without
requiring the buddy list window to be open at all times.</p>

<p>Pidgin is under constant development, and releases are usually frequent.
The latest news regarding Pidgin can be found on the <a href="http://planet.pidgin.im/">news</a>
page.</p>

<h1>What is Finch?</h1>

<p>Finch is the text-based version of Pidgin. It supports the same IM
networks, but you can run it in a console window. You can use it on Linux,
BSD, and other Unixes.</p>

<h1>What is libpurple?</h1>

<p>libpurple is the programming library that powers Pidgin and Finch. It's
responsible for connecting to all the IM networks, and for managing your
accounts and preferences. It's written in C and makes heavy use of <a
href="http://www.gtk.org/">Glib</a>.</p>

<h1>Is all of this free?</h1>

<p>We believe in freedom of communication.  To support our aspirations of
"IM Freedom," we release Pidgin, Finch, and libpurple as free software
under the <a href="http://www.gnu.org/licenses/gpl-2.0.txt">GNU General
Public License</a> (GPL).  We believe that giving others the freedom to modify,
share, and augment our code contributes to the goal of bringing freedom of
communication to the Internet.  The GPL allows us to ensure that any
modifications to our code remain free, so that everyone may enjoy their
benefits.</p>

<h1>How can I help?</h1>

<p>We always welcome feedback and contributions.  You don't need to be a
developer to help out, but if you are, you can help us by fixing bugs in our
code or building new functionality into it.  Our
<a href="http://developer.pidgin.im/">development site</a> includes numerous
resources for getting started with libpurple, Pidgin, and Finch development.</p>

<p>If you are a regular user, we encourage you to let us know about any
problems you encounter and to provide us with suggestions for improvement.
You can do so via our 
<a href="http://developer.pidgin.im">support system</a>,
<a href="irc://irc.freenode.net/pidgin">IRC channel</a>,
<a href="xmpp:devel@conference.pidgin.im/?join">XMPP conference</a>,
<a href="/cgi-bin/mailman/listinfo/support">support mailing list</a>, or
<a href="/cgi-bin/mailman/listinfo/devel">development mailing list</a>.  We
also encourage users to help one another solve problems and discover new
features using any of these media.</p>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
