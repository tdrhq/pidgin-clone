<?
	$page->title = 'Screenshots';
	require('inc/template.inc.php');
?>

<?	
$new_screenshots = array(
	"wysiwyg.png" => array(
		"width"       => 398,
		"height"      => 332,
		"thumbnail"   => "wysiwyg_t.png",
		"thumbwidth"  => 130,
		"thumbheight" => 108,
		"text"        => "You can now format your messages with <acronym title=\"What You See Is What ".
		                 "You Get\">WYSIWYG</acronym> capibilities---no more distracting HTML tags."
	),
	"minimal.png" => array(
		"width"       => 270,
		"height"      => 252,
		"thumbnail"   => "minimal_t.png",
		"thumbwidth"  => 130,
		"thumbheight" => 121,
		"text"        => "Pidgin's interface keeps getting simpler and more elegant.  It allows you to do ".
		                 "what you want, without getting in your way."
	),
	"persons.png" => array(
		"width"       => 309,
		"height"      => 435,
		"thumbnail"   => "persons_t.png",
		"thumbwidth"  => 92,
		"thumbheight" => 130,
		"text"        => "Have buddies with more than one IM account?  You can combine them so that they ".
		                 "take only a single spot on your buddy list which you can expand when necessary."
	),
	"yahoo_icon.png" => array(
		"width"       => 346,
		"height"      => 321,
		"thumbnail"   => "yahoo_icon_t.png",
		"thumbwidth"  => 130,
		"thumbheight" => 121,
		"text"        => "Pidgin fully supports buddy icons on AIM, ICQ, MSN, and Yahoo!."
	),
	"prefs.png" => array(
		"width"       => 722,
		"height"      => 597,
		"thumbnail"   => "prefs_t.png",
		"thumbwidth"  => 130,
		"thumbheight" => 107,
		"text"        => "Pidgin is as configurable as it needs to be.  It provides plenty of options to suit ".
		                 "your tastes, but doesn't bog you down with needless options you're not likely to care about."
	)
);

$screenshots = array(
	"account.png" => array(
		"width"       => 562,
		"height"      => 281,
		"thumbnail"   => "account_t.png",
		"thumbwidth"  => 130,
		"thumbheight" => 65,
		"text"        => "This is the new account editor.  Note that signing on a new " .
		                 "account takes only a single click."
	),
	"i18n.png" => array(
		"width"       => 436,
		"height"      => 484,
		"thumbnail"   => "i18n_t.png",
		"thumbwidth"  => 117,
		"thumbheight" => 130,
		"text"        => "Pidgin 0.60 has infinitely improved its internationalization support.  In this ".
		                 "screenshot, we see a conversation over AIM using in many different languages, ".
		                 "even using multiple character sets in a single message.  No other third-party ".
		                 "IM client can claim this kind of internationalization support."
	),
	"themes.png" => array(
		"width"       => 568,
		"height"      => 598,
		"thumbnail"   => "themes_t.png",
		"thumbwidth"  => 123,
		"thumbheight" => 130,
		"text"        => "The smileys you see in your conversations are themeable.  Writing a smiley ".
		                 "theme is easy, and installing one is even easier.  You can drag a link right ".
		                 "from the <a href=\"http://sourceforge.net/tracker/?atid=746976&amp;group_id=235&amp;func=browse\">Smiley Tracker</a> in your browser into the ".
		                 "themes selector."
	),
	"ft.png" => array(
		"width"       => 423,
		"height"      => 516,
		"thumbnail"   => "ft_t.png",
		"thumbwidth"  => 107,
		"thumbheight" => 130,
		"text"        => "File transfer support is one of the many new features in Pidgin 0.60."
	),
	"docklet.png" => array(
		"width"       => 489,
		"height"      => 442,
		"thumbnail"   => "docklet_t.png",
		"thumbwidth"  => 131,
		"thumbheight" => 118,
		"text"        => "Pidgin's GNOME applet has been replaced with a tray icon plugin.  This tray icon ".
		                 "will dock into any <a href=\"http://www.freedesktop.org\">freedesktop.org</a> ".
		                 "compliant notification area (GNOME 2, KDE 3.1) and provides rapid access to ".
		                 "common features and the ability to reduce clutter on your desktop by hiding ".
		                 "the buddy list window."
	),
	"newlist.png" => array(
		"width"         => 331,
		"height"        => 835,
		"thumbnail"     => "newlist-t.png",
		"thumbwidth"    => 49,
		"thumbheight"   => 124,
		"text"          => "The buddy list can optionally show your buddies buddy icons.  This makes ".
		                   "locating your buddies in a long list much easier.  When buddy icons are shown ".
		                   "in the list, a second line will provide more information about the buddy ".
		                   "(status messages, idle times, warning levels, etc.)"
	)
);


if (!isset($_GET['file'])) {
	?>
	<h1>Screenshots</h1>
	<table>
	<tr>
	<?

	$count = 0;		
	foreach($new_screenshots as $file => $shot) {
		$count++;
		?>	
		<td style="text-align: center; border: 0;">
		<a href="?file=<?=$file?>">
		<img src="/images/screenshots/<?=$shot["thumbnail"]?>" width="<?=$shot["thumbwidth"]?>" height="<?=$shot["thumbheight"]?>" border="0" alt="Screen Shot" />
		</a>
		<br />
		<p><?=$shot["text"]?></p>
		</td>
		<?
		if ($count % 2 == 0) print "</tr><tr>";
	}

	?>
	</tr>
	</table>

	<h2>Older Screenshots</h2>
	<table>
	<?

	$count = 0;		
	foreach($screenshots as $file => $shot) {
		if ($count % 2 == 0) print "<tr>";
		$count++;
		?>	
		<td style="text-align: center; border: 0;">
		<a href="?file=<?=$file?>">
		<img src="/images/screenshots/<?=$shot["thumbnail"]?>" width="<?=$shot["thumbwidth"]?>" height="<?=$shot["thumbheight"]?>" border="0" alt="Screen Shot" />
		</a>
		<br />
		<p><?=$shot["text"]?></p>
		</td>
		<?
		if ($count % 2 == 0) print "</tr>";
	}

	?>
	</table>
	<?

} else {
	$file = $_GET['file'];

	?>
	<h1>Screenshot</h1>
	<?

	$shot = null;
	if (isset($screenshots[$file])) {
		$shot =& $screenshots[$file];
	} else if (isset($new_screenshots[$file])) {
		$shot =& $new_screenshots[$file];
	} else {		
		print "Invalid screen shot.";
	}

	if ($shot) {
		?>
		<div align="center">
		<img src="/images/screenshots/<?=$file?>" width="<?=$shot["width"]?>" height="<?=$shot["height"]?>" border="0" alt="Screen Shot" />
		<br />
		<p><?=$shot["text"]?></p>
		</div>
		<?
	}
}

?>
