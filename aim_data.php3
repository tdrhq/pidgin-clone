<?php

// Written by temas. (Not that I think anyone would particularly have
// any use for this script)

# If AOL ever breaks this script, uncomment the lines below and fill in
# the appropriate values for the hash. That way, at least people can
# use Gaim until we figure out how the script is broken and fix it.

#printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x44, 0xa9, 0x5d, 0x26,
#                                           0xd2, 0x49, 0x04, 0x23,
#                                           0x93, 0xb8, 0x82, 0x1f,
#                                           0x51, 0xc5, 0x4b, 0x01);
#exit;

$aim_binary_dir = "/home/groups/g/ga/gaim/aim51";

# If the length is 0 then Gaim should handle it itself. These values
# should never be less than zero since they can only be 16 bits, and
# Gaim stores them as long ints.
if ($offset < 0 || $len <= 0) {
	exit;
}
# If AOL doesn't tell us what module (.ocm file) to get the data from,
# we use aim.exe
if (strlen($modname) == 0) {
	$modname = "aim.exe";
} else {
	# Gaim doesn't send the .ocm part. I don't know why. I forgot.
	$origmod = $modname; # We need to save this for one of our special cases.
	$modname = $modname . ".ocm";
}

$filename = $aim_binary_dir . "/" . basename($modname);

# If the given offset or length is larger than the size of the file, it's
# it's an invalid request, and so we hash the request. There's gotta be a
# prettier way of doing this, someone, please tell me what it is. This was
# copied from faimtest and translated from C to PHP.
$st = stat($filename);

if (($st) && ($offset > $st[7] || $len > $st[7])) {
	$buf[0] = $offset & 0xff;
	$buf[1] = ($offset >> 8) & 0xff;
	$buf[2] = ($offset >> 16) & 0xff;
	$buf[3] = ($offset >> 24) & 0xff;
	$buf[4] = $len & 0xff;
	$buf[5] = ($len >> 8) & 0xff;
	$buf[6] = ($len >> 16) & 0xff;
	$buf[7] = ($len >> 24) & 0xff;
	$data = sprintf("%c%c%c%c%c%c%c%c", $buf[0], $buf[1], $buf[2], $buf[3],
					    $buf[4], $buf[5], $buf[6], $buf[7]);

	print pack("H*", md5($data));
	exit;
}

# Oh, another fun special case ... Hacked by Rob.  If it breaks, yell at me.
# Essentialy, if they give us a bogus filename, we hash the module name + 
# the offset and length and return that.  Pretty fun stuff.

if (!$st)
{
	$buf[0] = $offset & 0xff;
	$buf[1] = ($offset >> 8) & 0xff;
	$buf[2] = ($offset >> 16) & 0xff;
	$buf[3] = ($offset >> 24) & 0xff;
	$buf[4] = $len & 0xff;
	$buf[5] = ($len >> 8) & 0xff;
	$buf[6] = ($len >> 16) & 0xff;
	$buf[7] = ($len >> 24) & 0xff;

	$data = sprintf("%s%c%c%c%c%c%c%c%c", $origmod, $buf[0], $buf[1], $buf[2], $buf[3],                                    $buf[4], $buf[5], $buf[6], $buf[7]);

	print pack("H*", md5($data));
	exit;
}

# If we can start reading before the end of the file, make sure we don't try
# to read past the end of the file.
if ($offset + $len > $st[7]) {
	$len = $st[7] - $offset;
}

# The length is never more than 4096. Yes, taking the modulo is correct. (At
# least according to libfaim.)
$len %= 4096;

# Yet another special case. If the length they're requesting is 0, then we
# hash NULL. Isn't that swell.
if ($len == 0) {
	print pack("H*", md5($NULL));
	exit;
}

$fd = fopen($filename, "r");
if ($fd <= 0) {
	exit;
}
if (fseek($fd, $offset, SEEK_SET) == -1) {
	fclose($fd);
	exit;
}
$buf = fread($fd, $len);
if (strlen($buf) == 0) {
	fclose($fd);
	exit;
}
fclose($fd);

print pack("H*", md5($buf));

exit;

?>
