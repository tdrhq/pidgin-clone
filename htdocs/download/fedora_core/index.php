<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin for Fedora Core";
  $page['section'] = "Download";
  $page['subsect'] = "Fedora Core";
  $page['description'] = "Download the free Pidgin universal chat client for Fedora Core";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://rpm.pidgin.im/fedora/pidgin.repo"
onclick="javascript:pageTracker._trackPageview('/goals/downloads/fedora_core');"
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
<span class="download_header">Download Pidgin</span><br />
<span class="number"><? echo $pidgin_version; ?> for Fedora Core</span>
</a></p>

<p>To install Pidgin on Fedora Core 4, 5, 6 or 7 save the pidgin.repo file from
above in /etc/yum.repos.d/</p>

<p>You can then use "yum install pidgin", or the Package Manager (pirut) to install Pidgin.</p>

<p><i>We do <b>not</b> provide packages for Fedora 8 or later.  If you use Fedora 8 or later,
please wait for the Fedora developers to release updated packages.</i></p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
