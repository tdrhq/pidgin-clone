<?
  // Update these to match the current page.
  $page['title'] =   "Fedora Core | Download";
  $page['section'] = "Download";
  $page['subsect'] = "Fedora Core";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://rpm.pidgin.im/fedora/pidgin.repo">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number">Pidgin <? echo $pidgin_version; ?></span>
</a></p>

<p>To install Pidgin on Fedora Core 4, 5 or 6, save the pidgin.repo file from
above in /etc/yum.repos.d/</p>

<p>You can then use "yum install pidgin", or the Package Manager (pirut) to install Pidgin.</p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>