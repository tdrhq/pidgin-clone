<?
	$page->title = 'Security Issues';
	require('template.inc.php');
?>

<?
$vulnerabilities = array(
	array(
		"title"        => "MSN strncpy buffer overflow",
		"date"         => "22 August 2004",
		"cve"          => "CAN-2004-0500",
		"summary"      => "Possible for a remote MSN user to cause a buffer overflow.",
		"description"  => "In two places in the MSN protocol plugins (object.c and slp.c), strncpy was used incorrectly; the size of the array was not checked before copying to it.  Both bugs affect MSN's MSNSLP protocol, which is peer-to-peer, so this could potentially be easy to exploit.",
		"fix"          => "Bounds checking was added in both places.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sebastian Krahmer, SUSE Security Team"
	),
	array(
		"title"        => "Smiley theme installation lack of escaping",
		"date"         => "22 August 2004",
		"cve"          => "CAN-2004-0784",
		"summary"      => "Dragging a carefully crafted smiley theme filename onto Gaim could cause arbitrary command execution.",
		"description"  => "To install a new smiley theme, a user can drag a tarball from a graphical file manager, or a hypertext link to one from a web browser.  When a tarball is dragged, Gaim executes a shell command to untar it.  However, it does not escape the filename before sending it to the shell.  Thus, a specially crafted filename could execute arbitrary commands if the user could be convinced to drag a file into the smiley theme selector.",
		"fix"          => "Filenames are now escaped using g_shell_quote().",
		"fixedversion" => "0.82",
		"discoveredby" => "A Gaim Crazy Patch Writer"
	),
	array(
		"title"        => "Groupware message receive integer overflow",
		"date"         => "26 August 2004",
		"cve"          => "CAN-2004-0754",
		"summary"      => "Carefully crafted messages could cause a buffer overflow.",
		"description"  => "Integer overflow in memory allocation results in heap overflow.  By passing the size variable as ~0, integer overflows to 0 when 1 is added in g_alloc().   a malloc(0) call results in 16 bytes of memory being allocated on IA- 32.  Then we can overflow the heap when nm_read_all() is called next step.  usually cases like this suck for exploitation, because the len (~0) is so large that a following call to memcpy() or strcpy() will just run into kernel mem or unmapped address  and fault.  however in this case we read the data from the network via a read() call, so we can just stop sending data and close the connection to short out before ~0 bytes are read.  however, this is triggered by input from the server, not directly from a client.  someone running a malicious groupware server could leverage this to run arbitrary code on the client.",
		"fix"          => "Bounds checking was added.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sean (\"infamous42md\")"
	),
	array(
		"title"        => "URL decode buffer overflow",
		"date"         => "26 August 2004",
		"cve"          => "CAN-2004-0785",
		"summary"      => "Receiving exceedingly long URLs can cause a buffer overflow.",
		"description"  => "Buffer overflow.  The URL is decoded into a static buffer of length 2048 bytes.  I'm not sure it's possible to receive a URL longer than 2048 bytes, as many protocols have message limits that are shorter than that.",
		"fix"          => "A check to make sure the source string is shorter than 2048 bytes is performed.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sean (\"infamous42md\")"
	),
	array(
		"title"        => "Local hostname resolution buffer overflow",
		"date"         => "26 August 2004",
		"cve"          => "CAN-2004-0785",
		"summary"      => "Possible buffer overflow when resolving the hostname of the local computer.",
		"description"  => "Buffer overflow.  If the local computers host name is not in /etc/hosts, and the computer performs a DNS query to obtain it's hostname when signing on to zephyr, it could receive a reply with a hostname greater than MAXHOSTNAMELEN (generally 64 bytes).  If gethostbyname() does not ensure the size of hostent->h_name is less than MAXHOSTNAMELEN, this value would be copied to a buffer that is not large enough.",
		"fix"          => "The calls to copy the hostname were replaced with calls that check the length of the destination buffer.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sean (\"infamous42md\")"
	),
	array(
		"title"        => "RTF message buffer overflow",
		"date"         => "26 August 2004",
		"cve"          => "CAN-2004-0785",
		"summary"      => "Invalid rich text format messages could cause a buffer overflow.",
		"description"  => "Buffer overflow.  There are some loops that read into fixed-sized buffers and do not check to make sure they are not writing too much.",
		"fix"          => "Added bounds checking to the two loops.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sean (\"infamous42md\")"
	),
	array(
		"title"        => "Content-length DOS (malloc error)",
		"date"         => "26 August 2004",
		"summary"      => "Posibile for a malicious web server to provide a web page with a false content-length value which could crash Gaim.",
		"description"  => "Remote crash.  When a remote server provides a large \"content-length\" header value, Gaim will attempt to allocate a buffer to store the content, however this allocation attempt will cause Gaim to crash if the length exceeds the amount of possible memory.  This happens when reading profile information on some protocols.  It also happens when smiley themes are installed via drag and drop.",
		"fix"          => "The call to g_malloc() was replaced with a call to g_try_malloc().  If the memory could not be allocated the function returns instead of causing the application to crash.",
		"fixedversion" => "0.82",
		"discoveredby" => "Sean (\"infamous42md\")"
	),
	array(
		"title"        => "MSN File transfer DOS (malloc error)",
		"date"         => "19 October 2004",
		"summary"      => "Crash when accepting a large file transfer that exceeds the amount of available memory.",
		"description"  => "Remote crash. After accepting a file transfer request, Gaim will attempt to allocate a buffer of a size equal to the entire filesize, this allocation attempt will cause Gaim to crash if the size exceeds the amount of available memory.",
		"fix"          => "Don't allocate a buffer for file transfers.",
		"fixedversion" => "1.0.2",
		"discoveredby" => "Gaim"
	),
	array(
		"title"        => "MSN SLP DOS (malloc error)",
		"date"         => "19 October 2004",
		"summary"      => "Crash when receiving malformed MSN SLP message",
		"description"  => "Remote crash. Gaim allocates a buffer for the payload of each message received based on the size field in the header of the message. A malicious peer could specify an invalid size that exceeds the amount of available memory.",
		"fix"          => "Replace call to g_malloc() with call to g_try_malloc(). If the memory could not be allocated the function returns instead of causing the application to crash.",
		"fixedversion" => "1.0.2",
		"discoveredby" => "Gaim"
	),
	array(
		"title"        => "MSN SLP buffer overflow",
		"date"         => "19 October 2004",
		"cve"          => "CAN-2004-0891",
		"summary"      => "Buffer overflow when receiving unexpected sequence of MSN SLP messages",
		"description"  => "Buffer overflow. memcpy was used without checking the size of the buffer before copying to it. Additionally, a logic flaw was causing the wrong buffer to be used as the destination for the copy under certain circumstances.",
		"fix"          => "Correct the logic to select the correct buffer, and add bounds checking to prevent malformed messages causing a buffer overflow.",
		"fixedversion" => "1.0.2",
		"discoveredby" => "Gaim"
	),
	array(
		"title"        => "AIM/ICQ remote denial of service",
		"date"         => "17 February 2005",
		"cve"          => "CAN-2005-0472",
		"summary"      => "Client freezes when receiving certain invalid messages",
		"description"  => "Certain malformed SNAC packets sent by other AIM or ICQ users can trigger an infinite loop in Gaim when parsing the SNAC.  The remote user would need a custom client, able to generate malformed SNACs.",
		"fix"          => "The OSCAR protocol plugin was modified to drop these malformed packets.",
		"fixedversion" => "1.1.3",
		"discoveredby" => "Brandon Scott (\"Xeon\")"
	),
	array(
		"title"        => "Remote DoS on receiving malformed HTML",
		"date"         => "17 February 2005",
		"cve"          => "CAN-2005-0473",
		"summary"      => "Client crashes when receiving specific malformed HTML",
		"description"  => "Remote crash. Receiving malformed HTML can result in an invalid memory access causing Gaim to crash.",
		"fix"          => "The HTML parsing functions were modified to correctly parse the malformed HTML.",
		"fixedversion" => "1.1.3",
		"discoveredby" => "Gaim"
	),
	array(
		"title"        => "Remote DoS on receiving malformed HTML",
		"date"         => "24 February 2005",
		"cve"          => "CAN-2005-0208",
		"summary"      => "Client crashes when receiving specific malformed HTML",
		"description"  => "Remote crash. Receiving malformed HTML can result in an invalid memory access causing Gaim to crash.",
		"fix"          => "The HTML parsing functions were modified to correctly parse the malformed HTML.",
		"fixedversion" => "1.1.4",
		"discoveredby" => "Gaim"
	),
	array(
		"title"        => "Remote DoS on receiving malformed HTML",
		"date"         => "2 April 2005",
		"cve"          => "CAN-2005-0965",
		"summary"      => "Client crash when receiving specific malformed HTML",
		"description"  => "The gaim_markup_strip_html function in Gaim 1.2.0, and possibly earlier versions, allows remote attackers to cause a denial of service (application crash) via a string that contains malformed HTML, which causes an out-of-bounds read.",
		"fix"          => "The function was changed to not allow the out-of-bounds read.",
		"fixedversion" => "1.2.1",
		"discoveredby" => "Jean-Yves Lefort"
	),
	array(
		"title"        => "Remote DoS on receiving certain messages over IRC",
		"date"         => "2 April 2005",
		"cve"          => "CAN-2005-0966",
		"summary"      => "Client crash and other strange behavior when receiving specific messages over IRC",
		"description"  => "The IRC protocol plugin in Gaim 1.2.0, and possibly earlier versions, allows (1) remote attackers to inject arbitrary Gaim markup via irc_msg_kick, irc_msg_mode, irc_msg_part, irc_msg_quit, (2) remote attackers to inject arbitrary Pango markup and pop up empty dialog boxes via irc_msg_invite, or (3) malicious IRC servers to cause a denial of service (application crash) by injecting certain Pango markup into irc_msg_badmode, irc_msg_banned, irc_msg_unknown, irc_msg_nochan functions.",
		"fix"          => "The IRC protocol plugin was modified to escape appropriate messages passed to the Gaim core.",
		"fixedversion" => "1.2.1",
		"discoveredby" => "Jean-Yves Lefort"
	),
	array(
		"title"        => "Jabber remote crash",
		"date"         => "4 April 2005",
		"cve"          => "CAN-2005-0967",
		"summary"      => "A remote jabber user can cause Gaim to crash by sending a specific file transfer request.",
		"description"  => "Sending a Gaim Jabber user a certain invalid file transfer request triggers an out-of-bounds read which causes Gaim to crash.",
		"fix"          => "The invalid file transfer request is ignored.",
		"fixedversion" => "1.2.1",
		"discoveredby" => "Marco Alvarez"
	),
	array(
		"title"        => "Remote crash on some protocols",
		"date"         => "10 May 2005",
		"cve"          => "CAN-2005-1261",
		"summary"      => "Specially crafted messages on certain protocols can cause a buffer overflow",
		"description"  => "It is possible for a remote user to overflow a static buffer by sending an IM containing a very large URL (greater than 8192 bytes) to the Gaim user.  This is not possible on all protocols, due to message length restrictions.  Jabber are SILC are known to be vulnerable.",
		"fix"          => "The URL parsing function was modified to not use a static buffer.",
		"fixedversion" => "1.3.0",
		"discoveredby" => "Stu Tomlinson"
	),
	array(
		"title"        => "MSN Remote DoS",
		"date"         => "10 May 2005",
		"cve"          => "CAN-2005-1262",
		"summary"      => "Remote denial of service when receiving a specially crafted MSN message",
		"description"  => "Potential remote denial of service bug resulting from not checking a pointer for non-NULL before passing it to strncmp, which results in a crash.  This can be triggered by a remote client sending an SLP message with an empty body.",
		"fix"          => "Check for NULL before attempting to use the pointer.",
		"fixedversion" => "1.3.0",
		"discoveredby" => "Siebe Tolsma"
	),
	array(
		"title"        => "Remote Yahoo! crash",
		"date"         => "10 June 2005",
		"cve"          => "CAN-2005-1269",
		"summary"      => "Remote denial of service on Yahoo!",
		"description"  => "Remove denial of service when being offered files with names containing non-ASCII characters.",
		"fix"          => "Attempt to convert the file name to a usable encoding, or fail gracefully in the case of an invalid file name.",
		"fixedversion" => "1.3.1",
		"discoveredby" => "Jacopo Ottaviani"
	),
	array(
		"title"        => "MSN Remote DoS",
		"date"         => "10 June 2005",
		"cve"          => "CAN-2005-1934",
		"summary"      => "Certain malformed MSN messages can cause gaim to crash",
		"description"  => "Remote attackers can cause a denial of service (crash) via a malformed MSN message that leads to a memory allocation of a large size, possibly due to an integer signedness error.",
		"fix"          => "Added a check for the invalid message.",
		"fixedversion" => "1.3.1",
		"discoveredby" => "Hugo de Bokkenrijder"
	),
	array(
		"title"        => "Gadu-Gadu memory alignment bug",
		"date"         => "11 August 2005",
		"cve"          => "CAN-2005-2370",
		"summary"      => "A memory alignment bug in the Gadu-Gadu protocol plugin can result in a buffer overflow",
		"description"  => "There was a memory alignment bug in the library Gaim uses to access the Gadu-Gadu network.  This bug can not be exploited on x86 architectures.  This bug was recently fixed in the libgadu library, but also needed to be fixed in Gaim because Gaim includes a copy of the libgadu library.",
		"fix"          => "The vulnerable section of code was modified to work correctly on all architectures.",
		"fixedversion" => "1.5.0",
		"discoveredby" => "Marcin Owsiany and Wojtek Kaniewski"
	),
	array(
		"title"        => "AIM/ICQ non-UTF-8 filename crash",
		"date"         => "11 August 2005",
		"cve"          => "CAN-2005-2102",
		"summary"      => "Invalid filenames can cause a crash on some systems",
		"description"  => "A remote user could cause Gaim to crash on some systems by sending the Gaim user a file whose filename contains certain invalid characters.  It is unknown what combination of systems are affected, but it is suspected that Windows users and systems with older versions of GTK+ are especially susceptible.",
		"fix"          => "The filename is validated as UTF-8 before Gaim attemps to display it.",
		"fixedversion" => "1.5.0",
		"discoveredby" => "Unknown"
	),
	array(
		"title"        => "AIM/ICQ away message buffer overflow",
		"date"         => "11 August 2005",
		"cve"          => "CAN-2005-2103",
		"summary"      => "Remote users can cause a buffer overflow",
		"description"  => "A remote AIM or ICQ user can cause a buffer overflow in Gaim by setting an away message containing many AIM substitution strings (such as %t or %n).",
		"fix"          => "The substitution function was modified to use a dynamic buffer instead of one with a fixed size.",
		"fixedversion" => "1.5.0",
		"discoveredby" => "Brandon Perry"
	)
);
/*	Template for the unfortunate future
	array(
		"title"        => "",
		"date"         => "",
		"cve"          => "",
		"summary"      => "",
		"description"  => "",
		"fix"          => "",
		"fixedversion" => "",
		"discoveredby" => ""
	),
*/

?>


<?
if (isset($_GET["id"])) {
	$id = intval($_GET["id"]); // Read in the ID of the vulnerability the user wants to view
}

if (!isset($id) || !is_int($id) || ($id < 0) || ($id >= count($vulnerabilities))) {
	// vulnerability index
	?>

	<h1>Index of Vulnerabilities</h1>

	<p>
	This is a list of all potential Gaim security vulnerabilities discovered after August 1st, 2004.
	</p>

	<table>
	<tr>
		<th>Title</th>
		<th>CVE Name</th>
		<th>Date</th>
		<th>Fixed In</th>
	</tr>

	<?
	for ($i = count($vulnerabilities)-1; $i >= 0; $i--) {
		$vun =& $vulnerabilities[$i];
		?>
		<tr>
		<td><a href="?id=<?=$i?>"><?=$vun['title']?></a></td>
		<?
		if (isset($vun['cve'])) {
			?>
			<td><a href="http://cve.mitre.org/cgi-bin/cvename.cgi?name=<?=$vun['cve']?>"><?=$vun['cve']?></a></td>
			<?
		} else {
			?>
			<td>N/A</td>
			<?
		}
		?>
		<td><?=$vun['date']?></td>
		<td><?=$vun['fixedversion']?></td>
		</tr>
		<?
	}
	?>
	</table>
	<?

} else {
	// individual vulnerability
	$vun =& $vulnerabilities[$id];

	?>
	<h1>Gaim Vulnerability</h1>

	<table>
		<tr><th align="right" valign="top">Title</th><td><?=$vun['title']?></td></tr>
		<tr><th align="right" valign="top">Date</th><td><?=$vun['date']?></td></tr>
		<?
		if (isset($vun['cve'])) {
			?>
			<tr><th align="right" valign="top">CVE Name</th><td><a href="http://cve.mitre.org/cgi-bin/cvename.cgi?name=<?=$vun['cve']?>"><?=$vun['cve']?></a></td></tr>
			<?
		} else {
			?>
			<tr><th align="right" valign="top">CVE Name</th><td>N/A</td></tr>
			<?
		}
		?>
		<tr><th align="right" valign="top">Discovered By</th><td><?=$vun['discoveredby']?></td></tr>
		<tr><th align="right" valign="top">Summary</th><td><?=$vun['summary']?></td></tr>
		<tr><th align="right" valign="top">Description</th><td><?=$vun['description']?></td></tr>
		<tr><th align="right" nowrap="true" valign="top">Fixed in Version</th><td><?=$vun['fixedversion']?></td></tr>
		<tr><th align="right" valign="top">Fix</th><td><?=$vun['fix']?></td></tr>
	</table>

	<p><a href="index.php">Return to Index of Vulnerabilities</a></p>
	<?
}

?>
