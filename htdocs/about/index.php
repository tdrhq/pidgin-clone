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

<h1>About Pidgin</h1>

<div style="float:right;">
<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download.inc");
?>
<br/>
<div style="text-align:center">
<img src="/shared/img/contact_window.png" alt="Buddy List" width="192" height="420" />
</div>
</div>

<p>Pidgin is a chat program which lets you log in to accounts on multiple
chat networks simultaneously. This means that you can be chatting with
friends on MSN, talking to a friend on Google Talk, and sitting in a Yahoo
chat room all at the same time.</p>

<p>Pidgin runs on Windows, Linux, and other UNIX operating systems. 
Looking for Pidgin for OS X? Try <a onclick="javascript:pageTracker._trackPageview('/external/adiumx.com');" href="http://www.adiumx.com/">Adium</a>!</p>

<p>Pidgin is compatible with the following chat networks out of the box: 
AIM, ICQ, Google Talk, Jabber/XMPP, MSN
Messenger, Yahoo!, Bonjour, Gadu-Gadu, IRC, Novell GroupWise Messenger, QQ,
Lotus Sametime, SILC, SIMPLE, MySpaceIM, and Zephyr.  It can support many
more with plugins.</p>

<p>Pidgin supports many features of these chat networks, such as file
transfers, away messages, buddy icons, custom smilies, and typing
notifications.  Numerous plugins also extend Pidgin's functionality above and
beyond the standard features.</p>

<p>Pidgin integrates with the system tray on Windows, GNOME2, and KDE 3.1.</p>

<p>Pidgin is under constant development.  Releases are usually frequent and
driven by user contributions, such as <a href="/support/">bug reports</a> and
<a href="http://developer.pidgin.im">patches</a>.</p>

<p>Pidgin is free and contains no ads.  All our code is open source and
licensed under the GNU General Public License.  This means you can get Pidgin's
underlying code and modify it to suit your needs, as long as you publish the
changes you make for everyone to benefit from as well.</p>

<h2>Supported Languages</h2>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/langdetect.inc"); ?>

<p>Pidgin is translated into many different languages, thanks to the
generous contributions of our volunteer translators.

<?php if (strlen($language) > 0 && $language != "English") { ?>
  You may be glad to know that Pidgin is already translated into
  <strong><? echo $language ?></strong>!
<?php } ?>

</p>

<ul class="condensed">
<?php

foreach (array_keys($languages) as $lang){

echo '<li>'.$languages[$lang].'</li>';

}

?>
</ul>

<p style="clear: both;">If your native language is missing from this list or
you've found that a translation contains errors, you can help us!  Take a look
at our <a href="http://developer.pidgin.im/wiki/TipsForTranslators">tips for
translators</a>.</p>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
