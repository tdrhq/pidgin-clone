<?
  // Update these to match the current page.
  $page['title'] =   "Source | Download";
  $page['section'] = "Download";
  $page['subsect'] = "Source";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://downloads.sourceforge.net/pidgin/pidgin-<? echo $pidgin_version; ?>.tar.bz2">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number">pidgin-<? echo $pidgin_version; ?>.tar.bz2</span>
</a></p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
