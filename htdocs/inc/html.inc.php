<?php

    require "version.inc.php";

	function start_html($title = "") {
		global $HTTP_USER_AGENT;
        global $current_version;
		global $style;

		if ($title == "") {
			$title = "pidgin";
		}
		else {
			$title .= " - pidgin";
		}

		if (!strstr($HTTP_USER_AGENT, "Gecko") &&
		    !strstr($HTTP_USER_AGENT, "Mozilla/5")):
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<?php endif; ?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
 <head>
  <title><?php print $title; ?></title>
  <style type="text/css">
  <!--
    CODE, PRE { font-size: medium; }
	TD {
		font-family: Verdana, Tahoma, Arial;
		font-size: medium;
	}

	.dl_table {
		border-collapse: collapse;
		border-spacing: 0px;
		empty-cells: show;
		width: 100%;
	}

	.dl_heading {
		background-color: #f7c871;
		border: 1px solid #000000;
		padding: 2px;
		font-weight: bold;
	}

	.dl_selection {
		background-color: #f0f0f0;
		border: 1px solid #000000;
		font-weight: bold;
		padding: 2px;
		text-align: left;
		vertical-align: top;
	}

	.dl_download {
		background-color: #f0f0f0;
		border: 1px solid #000000;
		padding: 2px;
		text-align: left;
		vertical-align: top;
	}
	.ver_box {
		background-color: #f7c871;
		border: 1px #000;
		border-style: none none solid solid;
		font-style: italic;
		font-weight: bold;
		font-size: 96%;
		vertical-align: bottom;
		text-align: center;
		font-family: Verdana, Arial, Helvetica;
		padding-bottom: 3px;
		width: 58px;
		height: 21px;
	}
  -->
  </style>
  <link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
  <link rel="alternate" type="application/rss+xml" title="RSS" href="http://pidgin.im/rss.php/news" />
 </head>
 <body bgcolor="#FFFFFF" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">
  <table border="0" width="100%" cellspacing="0" cellpadding="0">
   <tr>
    <td width="100%" valign="top">
     <table border="0" cellspacing="0" cellpadding="0" width="100%">
      <tr>
       <td width="190" valign="top"><a href="/index.php"><img src="<?php print $style->img_dir ?>/penguin2.png" width="190" height="122" border="0" alt="pidgin" title="Pidgin"/></a></td>
       <td width="100%" align="right" valign="top">
        <table border="0" cellspacing="0" cellpadding="0" width="179">
         <tr>
          <td width="1"><img src="/images/transpixel.gif"
           width="1" height="25" border="0" alt="" /></td>
          <td width="119" valign="top"><img src="<?php print $style->img_dir ?>/links_tl.png"
           width="119" height="25" border="0" alt="" /></td>
          <td valign="top" class="ver_box">v<?php print $current_version ?></td>
         </tr>
         <tr>
          <td width="1"><img src="/images/transpixel.gif"
           width="1" height="80" border="0" alt="" /></td>
          <td background="<?php print $style->img_dir ?>/links_bg.png" align="right" valign="top"
           colspan="2">
           <img src="/images/transpixel.gif" width="1" height="105" border="0" alt="" align="left" />
           <img src="/images/transpixel.gif" width="1" height="5"
            border="0" alt="" /><br />
           <?php start_font() ?>
           Source: [<small><a
		   href="http://prdownloads.sourceforge.net/gaim/gaim-<?php
           print $current_version ?>.tar.gz?download">tarball</a>,
           <a href="http://prdownloads.sourceforge.net/gaim/gaim-<?php
           print $current_version ?>.tar.bz2?download">bz2ball</a></small>]&nbsp;<br />
           <small><a href="/downloads.php">Other Packages</a>&nbsp;</small>&nbsp;<br />
           <small><a href="/ChangeLog">ChangeLog</a>&nbsp;</small>&nbsp;<br />
           <?php end_font() ?>
          </td>
         </tr>
        </table>
       </td>
      </tr>
     </table>
<?php
		start_font();
	}

	function end_html() {
		global $PHP_SELF;
		global $navigation, $nav_count;
		global $style;

?>
</font></td>
    <td width="1" bgcolor="#000000"><img src="/images/transpixel.gif" width="1" height="1" border="0" alt="" /></td>
    <td width="160" valign="top" bgcolor="<?php print $style->sidebar_bg ?>" align="center" ><img
     src="/images/transpixel.gif" width="160" height="5" border="0" alt="" /><br /><?php

		$navfound = false;

?>

<?php


		while (list($name, $data) = each($navigation)) {
			print "<img\n";
			print "     src=\"" . $style->img_dir;
			print "/navbar/" . $data["PREFIX"] . "_t.png\"";
			print " width=\"140\" height=\"35\" border=\"0\" alt=\"\" /><a\n";
			print "     href=\"" . $data["URL"] . "\"><img\n";
			print "     src=\"" . $style->img_dir;
			print "/navbar/" . $data["PREFIX"];
			
			if (!$navfound && $data["URL"] == $PHP_SELF) {
				$navfound = true;

				print "_bh.png";
			}
			else {
				print "_b.png";
			}
			
			print "\" width=\"140\" height=\"20\" border=\"0\"";
			print " alt=\"$name\" title=\"$name\"/></a><br />";
		}
?>
<br />
<br />

<p><b>Pidgin is not endorsed by or affiliated with AOL, Google, ICQ, MSN or Yahoo!</b></p>

<p>
<a href="http://developer.pidgin.im/wiki/WhatIsLibpurple"><img alt="Powered by libpurple" border="0" src="/images/powered_by_libpurple.png"/></a>
<a href="http://sourceforge.net"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=235&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" /></a>
</p>
<br />
</td>
   </tr>
   <tr>
    <td colspan="3" bgcolor="#000000"><img src="/images/transpixel.gif" width="1" height="1" border="0" alt="" /></td>
   </tr>
   <tr>
    <td colspan="3">
     <blockquote>
      <p><font size="-1">
      </font></p>
      <p><font size="-1">
<?php
if ($txtfile = @file("sayings.txt")) {
	srand ((double) microtime() * 10000000);
	print $txtfile[array_rand($txtfile)];
}
?><br />
       Website design and development provided by
       <a href="http://www.portaldesign.net/">Portal Web Design</a>.<br />
	   Layout Copyright &copy; 2001-2004
	   <a href="http://www.portaldesign.net/">Portal Web Design</a>.
	   All rights reserved.
      </font></p>
     </blockquote>
    </td>
   </tr>
  </table>
 </body>
</html>
<?php
	}

	function start_section($title, $date = "", $doitalic = true) {
?>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
 <tr>
  <td width="100%"><img src="/images/transpixel.gif" width="1" height="5" border="0" alt="" /></td>
  <td width="10" rowspan="2" background="/images/news_left.png" valign="top"><img src="/images/news_tl.png" width="10" height="10" border="0" alt="" /></td>
  <td background="/images/news_top.png"><img src="/images/transpixel.gif" width="1" height="5" border="0" alt="" /></td>
 </tr>
 <tr>
  <td nowrap="nowrap" valign="bottom"><?php start_font(-2); ?>&nbsp;&nbsp;<?php
  	if ($do_italic)
		print "<i>";
	
	print $date;
	
	if ($do_italic)
		print "</i>";
?></font></td>
  <td bgcolor="#F7C871" nowrap="nowrap"><?php start_font(); ?><b><?php print $title; ?></b>&nbsp;&nbsp;&nbsp;</font></td>
 </tr>
 <tr>
  <td background="/images/date_bottom.png"><img src="/images/transpixel.gif" width="1" height="5" border="0" alt="" /></td>
  <td><img src="/images/news_mb.png" width="10" height="5" border="0" alt="" /></td>
  <td background="/images/news_bottom.png"><img src="/images/transpixel.gif" width="1" height="5" border="0" alt="" /></td>
 </tr>
</table>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0">
 <tr>
  <td width="30"><img src="/images/transpixel.gif" width="30" height="1" border="0" alt="" /></td>
  <td valign="top">
<?php
		start_font();
	}

	function end_section() {
		end_font();
?>
</td>
  <td width="30"><img src="/images/transpixel.gif" width="30" height="1" border="0" alt="" /></td>
 </tr>
</table>
<br />
<?php
	}

	function start_tag($name, $atts = 0)
	{
		print "<" . $name;

		if ($atts != 0)
		{
			while (list($var, $val) = each($atts))
				print " $var=\"$val\"";
		}

		print ">";
	}

	function end_tag($name)
	{
		print "</" . $name . ">";
	}

	function start_font($size = "-1") {
		print "<font face=\"Verdana, Tahoma, Arial, Helvetica\"";
		print " size=\"$size\">\n";
	}

	function end_font() {
		print "</font>";
	}

	function ahref($desc, $url)
	{
		print "<a href=\"$url\">$desc</a>";
	}

	function start_table($atts)
	{
		start_tag("table", $atts);
	}

	function end_table()
	{
		end_tag("table");
	}

	function start_tr()
	{
		print " ";

		start_tag("tr");
	}

	function end_tr()
	{
		end_tag("tr");
		print "\n";
	}

	function start_td($atts = 0)
	{
		print "  ";
		
		start_tag("td", $atts);

		start_font();
	}

	function end_td()
	{
		end_font();
		
		end_tag("td");
		print "\n";
	}

	function br()
	{
		print "<br />\n";
	}

	function empty_cell($atts = 0)
	{
		start_tag("td", $atts);
		print "&nbsp;";
		end_tag("td");
	}
?>
