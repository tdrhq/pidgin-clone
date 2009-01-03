<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin for Windows";
  $page['section'] = "Download";
  $page['subsect'] = "Windows";
  $page['description'] = "Download the free Pidgin universal chat client for Windows";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://downloads.sourceforge.net/pidgin/pidgin-<? echo $pidgin_version; ?>.exe"
onclick="javascript:pageTracker._trackPageview('/goals/downloads/win');"
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?> for Windows</span>
</a></p>

</div>

<p>If your GTK+ is already downloaded and up to date, we have a smaller <a href="http://downloads.sourceforge.net/pidgin/pidgin-<? echo $pidgin_version; ?>-no-gtk.exe">installer without GTK+</a>. </p>


</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
