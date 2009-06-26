<?
  // Update these to match the current page.
  $page['title'] =   "Download Pidgin for Ubuntu";
  $page['section'] = "Download";
  $page['subsect'] = "Ubuntu";
  $page['description'] = "Download the free Pidgin universal chat client for Ubuntu";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p>Ubuntu ships Pidgin but does not update it after a release (except for security issues and high-severity bugs). For those users who desire new releases of Pidgin, we have packaged Pidgin in a PPA. If you encounter problems with these packages, try <a href="../source">building from source</a> and <a href="/support">report the bug</a>.</p>
<p>To setup the PPA, copy-and-paste these commands into a <a href="https://help.ubuntu.com/community/UsingTheTerminal#Starting%20a%20Terminal">terminal</a>:<br /></p>

<p class='code'>sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com \
&nbsp;&nbsp;&nbsp;&nbsp;67265eb522bdd6b1c69e66ed7fb8bee0a1f196a8</p>
<p class='code'>echo deb http://ppa.launchpad.net/pidgin-developers/ppa/ubuntu \
&nbsp;&nbsp;&nbsp;&nbsp;`lsb_release --short --codename` main | \<br />
&nbsp;&nbsp;&nbsp;&nbsp;sudo tee /etc/apt/sources.list.d/pidgin-ppa.list</p>

<p>Once this PPA is setup, Pidgin updates will show up in Update Manager along with the usual Ubuntu updates. The PPA will need to be re-setup only after upgrading Ubuntu.</p>

<p>This PPA is maintained by one developer, so please be patient. It often lags behind the source releases a couple of days.</p>
</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
