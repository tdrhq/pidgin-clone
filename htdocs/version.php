<?php

header("Content-Type: text/plain");
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
error_reporting(0);

$changelog_file = 'ChangeLog';
$changelog_file_win32 = 'win32/ChangeLog.win32';

if(isset($_GET['version'], $_GET['build'])) {
	$version = $_GET['version'];
	$build = $_GET['build'];
} else {
	exit();
}

$dev = ((strpos($version, 'cvs') !== false) || (strpos($version, 'beta') !== false) || (strpos($version, 'dev') != false) || (strpos($version, 'devel') != false));
$win32 = ($build == 'purple-win32');
if (!$win32) {
	$win32 = ($build == 'gaim-win32');
}
$my_ver = str_replace('cvs', '', $version);
$my_ver = str_replace('devel', '', $my_ver);
$my_ver = str_replace('dev', '', $my_ver);
$my_ver = preg_replace('/beta(\d+)$/', '', $my_ver);
$my_ver = preg_replace('/-(\d+)$/', '', $my_ver);
$cur_ver = ($win32 ? $pidgin_win32_version : $pidgin_version);

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
    $text = parse_changelog($changelog_file_win32, $cur_ver, $my_ver);
    if (!preg_match("/^\s*$/", $text)) {
        print "Win32 Changes:\n\n";
        print $text;
    }
}
?>
