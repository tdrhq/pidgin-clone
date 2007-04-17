<?
	$page->title = 'Status Icons';
	require('inc/template.inc.php');
?>

<h1>Status Icons</h1>

<?
	$statusicons = array(
		"AIM" => array(
			"ID" => "aim",
			"ICONS" => array(
				"Online confirmed account"      => "confirmed.png",
				"Online unconfirmed account"    => "unconfirmed.png",
				"AOL user"                      => "aol.png",
				"Administrator"                 => "admin.png",
				"Away"                          => "away.png",
				"ActiveBuddy interactive agent" => "activebuddy.png",
				"Wireless user"                 => "wireless.png",
				"Offline"                       => "offline.png",
			)
		),

		"Gadu-Gadu" => array(
			"ID" => "gg",
			"ICONS" => array(
				"Online / Available" => "online.png",
				"Away"               => "away.png",
				"Busy"               => "busy.png",
				"Invisible"          => "invisible.png",
			)
		),

		"ICQ" => array(
			"ID" => "icq",
			"ICONS" => array(
				"Online / Available" => "online.png",
				"Away"               => "away.png",
				"Do not disturb"     => "dnd.png",
				"Not available"      => "na.png",
				"Occupied"           => "occ.png",
				"Free for chat"      => "ffc.png",
				"Offline"            => "offline.png",
			)
		),

		"IRC" => array(
			"ID" => "irc",
			"ICONS" => array(
				"Online / Available" => "online.png",
				"Offline"            => "offline.png",
			)
		),

		"Jabber" => array(
			"ID" => "jabber",
			"ICONS" => array(
				"Online / Available" => "available.png",
				"Away"               => "away.png",
				"Invitation to chat" => "chat.png",
				"Do not disturb"     => "dnd.png",
				"Presence error"     => "error.png",
				"Extended away"      => "xa.png",
				"Offline"            => "offline.png",
			)
		),

		"MSN" => array(
			"ID" => "msn",
			"ICONS" => array(
				"Online / Available" => "online.png",
				"Away"               => "away.png",
				"Occupied"           => "occupied.png",
				"You are not on their buddy list"
									 => "nr.png",
				"Blocked"            => "blocked.png",
				"Offline"            => "offline.png",
			)
		),

		"Yahoo!" => array(
			"ID" => "yahoo",

			"ICONS" => array(
				"Online / Available" => "online.png",
				"Away"               => "away.png",
				"Inviting to a game" => "game.png",
				"Idle"               => "idle.png",
				"Offline"            => "offline.png",
			)
		),

		"Chat" => array(
			"ID" => "chat",

			"ICONS" => array(
				"Founder / Owner"       => "founder.png",
				"Operator / Moderator " => "op.png",
				"&quot;Half-Op&quot;"   => "halfop.png",
				"Voiced user"           => "voice.png",
				"Ignored user"          => "ignored.png",
			)
		)
	);
?>

<ul>
<?
foreach($statusicons as $name => $data) {
	?>
	<li><a href="#<?=$data["ID"]?>"><?=$name?></a></li>
	<?
}
?>
</ul>

<?
foreach($statusicons as $name => $data) {
	?>
	<a name="<?=$data["ID"]?>"></a>
	<h2><?=$name?></h2>
	<?

	foreach ($data["ICONS"] as $desc => $filename) {
		?>
		<img src="/images/statusicons/<?=$data["ID"]?>/<?=$filename?>" alt="<?=$desc?>" />
		<?=$desc?><br />
		<?
	}
}

?>
