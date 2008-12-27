<?
  // Update these to match the current page.
  $page['title'] =   "About Pidgin, the universal chat client";
  $page['section'] = "About";
  $page['description'] = "General information about the Pidgin chat client.";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");

?>

<div id="content">





<div class="box_full">
<div id="main">

<h1>About Pidgin Chat</h1>

<div style="float:right;">
<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download.inc");
?>
<br/>
<div style="text-align:center">
<img src="/shared/img/contact_window.png" alt="Buddy List" width="192" height="420" />
</div>
</div>

<p>Pidgin is an chat program which lets you log in to accounts on multiple chat networks
simultaneously. This means that you can be chatting with friends on MSN,
talking to a friend on Google Talk, and sitting in an Yahoo channel all at
the same time.</p>

<p>Pidgin runs on Windows, Linux, and other UNIX operating systems. 
Looking for Pidgin for OS X? Try <a href="http://www.adiumx.com/">Adium</a>!</p>

<p>Pidgin is compatible with the following chat networks: 
AIM, ICQ, Google Talk, Jabber/XMPP, MSN
Messenger, Yahoo!, Bonjour, Gadu-Gadu, IRC, Novell GroupWise Messenger, QQ,
Lotus Sametime, SILC, SIMPLE, MySpaceIM, and Zephyr.</p>

<p>Pidgin supports almost all features of the various chat networks, such as file
transfer, away messages, and typing notifications.  Numerous plugins also extend Pidgin's functionality above and beyond the standard features.</p>

<p>Pidgin integrates with the system tray on Windows, GNOME2, and KDE 3.1.</p>

<p>Pidgin is under constant development, and releases are usually frequent.</p>

Pidgin is free!  And there are no ads.  All our code is open source and licensed under the GPL.

<h2>Supported Languages</h2>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/langdetect.inc"); ?>

<?php if (strlen($language) > 0) { ?>
  Pidgin supports <strong><? echo $language ?></strong>!<br/>
<?php } ?>

<ul class="condensed">
<?php

foreach (array_keys($languages) as $lang){

echo '<li>'.$languages[$lang].'</li>';

}

?>
</ul>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
