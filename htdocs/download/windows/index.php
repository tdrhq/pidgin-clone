<?
  // Update these to match the current page.
  $page['title'] =   "Windows | Download";
  $page['section'] = "Download";
  $page['subsect'] = "Windows";

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
onClick="javascript:pageTracker._trackPageview('/goals/downloads/win');"
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?> for Windows</span>
</a></p>

<p>This download is for the full installer for Windows, including GTK+. This is probably what you want.</p>

<p>If you already have GTK+ installed (for example, if you're running GIMP or another application using GTK+), we provide a smaller <a href="http://downloads.sourceforge.net/pidgin/pidgin-<? echo $pidgin_version; ?>-no-gtk.exe">installer without GTK+</a>. However, please be aware that we often ship updated copies of GTK+ and continuing to use an older version may cause problems with Pidgin. If you have problems, please re-install using the full installer and upgrade GTK+ before contacting us.</p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
