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
<img src="/shared/img/logo.pidgin.png" alt="[Pidgin Logo]" id="logo" />
<a href="/download/">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?></span></a>
</div>

</div>
</div>

<div class="box_home">
<div id="main">

<h1>What is Pidgin?</h1>

<p>Pidgin is a multi-protocol Instant Messaging client that allows you to use all of your IM accounts at once.</p>

<p>Pidgin can work with:</p>

<ul class="condensed">
  <li>AIM</li>
  <li>ICQ</li>
  <li>SIMPLE</li>
  <li>Bonjour</li>
  <li>IRC</li>
  <li>Sametime</li>
  <li>Gadu-Gadu</li>
  <li>MSN</li>
  <li>XMPP</li>
  <li>Google Talk</li>
  <li>QQ</li>
  <li>Yahoo!</li>
  <li>Groupwise</li>
  <li>SILC</li>
  <li>Zephyr</li>
</ul>

<p class="more"><a href="/about/"><img src="/shared/img/button.learn-more.gif" alt="Learn More" /></a></p>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
