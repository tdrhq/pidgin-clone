<?php
require 'inc/version.inc.php';
error_reporting(0);
/*
Is anyone still using the data anymore?

require 'inc/database.inc.php';
require 'inc/config.inc.php';
*/

$changelog_file = 'ChangeLog';
$changelog_file_win32 = 'win32/ChangeLog.win32';

if(isset($_GET['version'], $_GET['build'])) {
	$version = $_GET['version'];
	$build = $_GET['build'];
} else {
	exit();
}

$dev = ((strpos($version, 'cvs') !== false) || (strpos($version, 'beta') !== false) || (strpos($version, 'dev') != false));
$win32 = ($build == 'purple-win32');
if (!$win32) {
	$win32 = ($build == 'gaim-win32');
}
$my_ver = str_replace('cvs', '', $version);
$my_ver = str_replace('dev', '', $version);
$my_ver = preg_replace('/beta(\d+)$/', '', $my_ver);
$my_ver = preg_replace('/-(\d+)$/', '', $my_ver);
$cur_ver = ($win32 ? $current_win32_version : $current_version);

if ($cur_ver != $my_ver || $dev == false) {
	$cur_ver_parts = explode('.', $cur_ver);
	$my_ver_parts = explode('.', $my_ver);

	$upgrade = false;

	for($i=0; $i < count($cur_ver_parts); $i++) {
		if (!isset($my_ver_parts[$i]) || $cur_ver_parts[$i] > $my_ver_parts[$i]) {
			$upgrade = true;
		} else if ($my_ver_parts[$i] > $cur_ver_parts[$i]) {
			break;
		}
	}

	if($upgrade == false) {
		exit();
	}
}

/* This code is going to let me track how quickly people upgrade,
 * for my statistics project.  Feel free to put on your tinfoil hat */

/*
Is anyone still using the data anymore?

if(!$dev) {
	$db = new Database($sql_type, $sql_server, $sql_username, $sql_password, $sql_database);
	$db->connect();
	$db->query("UPDATE version_track SET count=count+1 WHERE day=NOW() AND version = '$my_ver' AND build='$build' AND cur_version = '$cur_ver'");
	if(mysql_affected_rows() == 0) {
		$db->query("INSERT INTO version_track (day,version,build,count,cur_version) VALUES(NOW(),'$my_ver','$build',1,'$cur_ver')");
	}
	$db->disconnect();
}
*/

function parse_changelog($filename, $cur_ver, $firstversion) {
    $f = fopen($filename, "r");
	$changelog_text = fread($f, filesize($filename));
	fclose($f);

	/* skip the Pimpin' Penguin line */
	$startpos = strpos($changelog_text, "version $cur_ver");
	$oldpos = strpos($changelog_text, "version $firstversion ", $startpos);

	if ($oldpos === false) {
		return '';
	}

	return substr($changelog_text, $startpos, $oldpos - $startpos);
}

print $cur_ver . "\n";
print parse_changelog($changelog_file, $cur_ver, $my_ver);

if ($win32) {
    print "Win32 Changes:\n\n";
    print parse_changelog($changelog_file_win32, $cur_ver, $my_ver);
}
?>
