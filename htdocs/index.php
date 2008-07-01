<?
  // Update these to match the current page.
  $page['title'] =   "Home";
  $page['section'] = "Home";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<div class="box_home">
<div id="left">

<div id="version"><img src="/shared/img/logo.text-big.gif" alt="Pidgin" />
<span class="number"><? echo $pidgin_version; ?></span>
</div>

<div id="download">
<img src="/shared/img/logo.pidgin.png" alt="[Pidgin Logo]" id="logo" width="107" height="185" />
<a id="download_link" href="/download/">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" width="48" height="48" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?></span></a>
</div>

<script type="text/javascript"><!--
if (navigator.platform.indexOf("Win") != -1)
	document.getElementById("download_link").href = "/download/windows/";
// --></script>


<p class="linkbar">
  <a href="http://developer.pidgin.im/wiki/ChangeLog">ChangeLog</a> |
  <a href="http://developer.pidgin.im/wiki/FAQ" title="Frequently Asked Questions">FAQ</a>
</p>

</div>

</div>

<div class="box_home" id="right">
<div id="main">

<h1>What is Pidgin?</h1>

<p>Pidgin is a multi-protocol Instant Messaging client that allows you to use all of your IM accounts at once.</p>

<p>Pidgin can work with:</p>

<ul class="condensed">
  <li>AIM</li>
  <li>Bonjour</li>
  <li>Gadu-Gadu</li>
  <li>Google Talk</li>
  <li>Groupwise</li>
  <li>ICQ</li>
</ul>
<ul class="condensed">
  <li>IRC</li>
  <li>MSN</li>
  <li>MySpaceIM</li>
  <li>QQ</li>
  <li>SILC</li>
</ul>
<ul class="condensed">
  <li>SIMPLE</li>
  <li>Sametime</li>
  <li>XMPP</li>
  <li>Yahoo!</li>
  <li>Zephyr</li>
</ul>

<p>Pidgin is free software.  It is licensed under the GNU General Public
License (GPL) version 2.  This means you are free to use it and to modify it,
but if you distribute your modifications you must distribute the modified
source code as well.</p>

<p class="more"><a href="/about/"><img src="/shared/img/button.learn-more.png" alt="Learn More" width="128" height="32"/></a></p>

</div>
</div>

<!-- This will pull from somewhere else at some point -->
<p class="more" id="lowblurb">We are aware of the problems connecting to ICQ with Pidgin
2.4.2 and older. We hope to have a new release and packages out soon to fix
this. In the meantime, if you compile Pidgin from source you can use this
<a href="pidgin-2.4.2-icq6.patch">patch</a> to fix it.</p>

<? /* Avoid outputting this stuff yet.
<!-- These are not ready to go yet -->
 <div class="box_full">

<div id="newsbrief">
Test
</div>

<div id="blogbrief">
Test
</div>

</div> */ ?>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
