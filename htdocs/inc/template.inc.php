<?
/*
This template displays the content pages of the site.
To load a page through this template, simply include the
following two lines at the top of the content file:

	$page->title = 'About';
	require('template.inc.php');
*/

if (defined('PAGE')) return;
ob_start();

require 'version.inc.php';
require "navigation.inc.php";
require "style.inc.php";

define('PAGE', $_SERVER['SCRIPT_NAME']);
$page->title = (isset($page->title)?$page->title.' - ':'').'Pidgin';
$page->file = substr($_SERVER['SCRIPT_NAME'], strrpos($_SERVER['SCRIPT_NAME'], '/') + 1);


?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title><?=$page->title?></title>
	<link rel="Stylesheet" href="/gaim.css" type="text/css" media="screen" />
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
	<link rel="alternate" type="application/rss+xml" title="RSS" href="http://pidgin.im/rss.php/news" />
	<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
	</script>
	<script type="text/javascript">
	_uacct = "UA-50914-1";
	urchinTracker();
	</script>
</head>

<body>

<div id="header">

<h1>
  <a href="/" title="Pidgin home page"><img src="/images/gaim-logo.png" height="46" width="35" alt="Pidgin logo" class="logo" />Pidgin</a>
  <span class="version"><?=$current_version?></span>
</h1>

<div class="subtitle">A multi-protocol instant messaging (IM) client</div>

<div id="download">
  Source: <a href="http://prdownloads.sourceforge.net/gaim/gaim-<?=$current_version?>.tar.gz?download">tarball</a>,
  <a href="http://prdownloads.sourceforge.net/gaim/gaim-<?=$current_version?>.tar.bz2?download">bz2ball</a><br />
  <a href="/downloads.php">Other Packages</a><br />
  <a href="/ChangeLog">ChangeLog</a> (<a href="/win32/ChangeLog.win32">Win32</a>)
</div>

</div>
<!-- end #header -->

<div id="nav" style="background: <?php print $style->sidebar_bg ?>;">
<ul>
<?
// display the menu
$navfound = false;
foreach ($navigation as $name => $data) {
	$srct = $style->img_dir . '/navbar/' . $data["PREFIX"] . '_t.png';
	$srcb = $style->img_dir . '/navbar/' . $data["PREFIX"];
	if (!$navfound && $data["URL"] == PAGE) {
		$navfound = true;
		$srcb .= '_bh.png';
	}
	else {
		$srcb .= '_b.png';
	}
	?>
	<li>
		<a href="<?=$data["URL"]?>">
			<img src="<?=$srct?>" width="140" height="35" border="0" alt="" />
			<img src="<?=$srcb?>" width="140" height="20" border="0" alt="<?=$name?>" />
		</a>
	</li>
	<?
}
?>
</ul><br />
<iframe src="http://rcm.amazon.com/e/cm?t=gaim-20&amp;o=1&amp;p=8&amp;l=as1&amp;asins=1590594673&amp;fc1=000000&amp;lc1=0000ff&amp;bc1=000000&amp;lt1=_top&amp;nou=1&amp;IS2=1&amp;f=ifr&amp;bg1=cdd7ea&amp;f=ifr" width="120" height="240" scrolling="no" marginwidth="0" marginheight="0" frameborder="0">
</iframe>
</div>
<!-- end #nav -->

<div id="content">
<?
// display the content
require($page->file);
?>
</div>
<!-- end #content -->

<div id="footer">

<p><strong>Pidgin is not endorsed by or affiliated with AOL, Google, ICQ, MSN or Yahoo!</strong></p>

<p>
<a href="http://developer.pidgin.im/wiki/WhatIsLibpurple"><img alt="Powered by libpurple" border="0" src="/images/powered_by_libpurple.png"/></a>
<a href="http://sourceforge.net"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=235&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" /></a>
</p>

</div>
<!-- end #footer -->

</body>
</html>

<?
// we've already included the content, so exit() to avoid displaying it again
ob_end_flush();
exit();
?>
