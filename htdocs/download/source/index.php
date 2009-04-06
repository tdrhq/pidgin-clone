<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin Source";
  $page['section'] = "Download";
  $page['subsect'] = "Source";
  $page['description'] = "Download the source code for the free Pidgin universal chat client";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://downloads.sourceforge.net/pidgin/pidgin-<? echo $pidgin_version; ?>.tar.bz2"
onclick="javascript:pageTracker._trackPageview('/goals/downloads/source');"
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
<span class="download_header">Download Pidgin</span><br />
<span class="number"><? echo $pidgin_version; ?> Source</span>
</a></p>

<p>This download is for the source code of Pidgin.</p>
<p>If you want to <b>use</b> Pidgin, you should first look for pre-built packages from us (see the links on the left) or your operating system distribution.</p>
<p>If you are looking to <b>modify</b> Pidgin, you may want to look at our <a href="http://developer.pidgin.im/wiki/UsingPidginMonotone">instructions for checking out the code from our repository</a>.</p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
