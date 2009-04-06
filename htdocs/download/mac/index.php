<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin for Mac OS X";
  $page['section'] = "Download";
  $page['subsect'] = "Mac OS X";
  $page['description'] = "Download the free Pidgin universal chat client for Mac OS X";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://pdb.finkproject.org/pdb/package.php/pidgin" onclick="javascript:pageTracker._trackPageview('/external/pdb.finkproject.org');">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
<span class="download_header">Find Pidgin</span><br />
<span class="number">for Mac OS X via Fink</span>
</a></p>

<p><i>Please note, we do <b>not</b> provide support for issues related to installing Fink or the Fink package of Pidgin. Any Fink-specific questions or concerns will need to be directed to the Fink project.</i></p>

<p>For information on installing Fink, visit the <a href="http://finkproject.org/download" onClick="javascript:pageTracker._trackPageview('/external/finkproject.org');">Fink download page</a>.</p>

<p>For a more native Mac OS X experience, you may be interested in using <a href="http://adiumx.com" onClick="javascript:pageTracker._trackPageview('/external/adiumx.com');">Adium</a> instead, which uses libpurple (the core of Pidgin) for much of its protocol support.</p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
