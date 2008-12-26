<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin, the universal IM client";
  $page['section'] = "Download";
  $page['description'] = "Download the free Pidgin IM client.  Pidgin runs on Windows and Linux/UNIX.";
  $page['subsect'] = "";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main" style="position:relative;">

<h1>Download Pidgin</h1>

<p>Download the free Pidgin IM client.  Pidgin runs on Windows and Linux/UNIX.</p>

<noscript>
<ul>
  <li><a href="/download/windows/">Windows</a></li>
  <li><a href="/download/source/">Source</a></li>
  <li><a href="/download/fedora_core">Fedora Core</a></li>
  <li><a href="/download/centos_rhel">CentOS / RHEL</a></li>
</ul>
</noscript>

<div id="download_wrapper" style="display:none">
<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download.inc");
?>
</div>
<script type="text/javascript"><!--
document.getElementById("download_wrapper").style.display = "";
// --></script>

<a href="http://developer.pidgin.im/wiki/ChangeLog">ChangeLog</a>


</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
