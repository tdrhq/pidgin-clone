<?
  // Update these to match the current page.
  $page['title'] =   "Download";
  $page['section'] = "Download";
  $page['subsect'] = "";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1>Download</h1>

<p>Pidgin runs on all Windows and UNIX platforms. In addition to the source
code, we distribute binaries for Windows, Fedora Core, and CentOS / RHEL.</p>

<ul>
  <li><a href="/download/windows/">Windows</a></li>
  <li><a href="/download/source/">Source</a></li>
  <li><a href="/download/fedora_core">Fedora Core</a></li>
  <li><a href="/download/centos_rhel">CentOS / RHEL</a></li>
</ul>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>