<?php
	function setup_site($title = "") {
		global $db, $session, $users;
		global $sql_type, $sql_server, $sql_username, $sql_password;
		global $sql_database;
		global $setup_ran;
		
		if ($setup_ran == false) {
			$setup_ran = true;

			/* Connect to the database. */
			$db = new Database($sql_type, $sql_server, $sql_username,
			                   $sql_password, $sql_database);
			$db->connect();

			/* Session management. */
			$session = new Session();
			$session->startSession();

			$users = new Users();
		}

	}

	function fatal_error($error) {
		/* Uncomment this to provide more verbose error reporting.
		   See also base.inc.php
		echo "<b>Fatal error:</b> " . $error . "<br />\n";
		site_shutdown();
		exit;
		*/
	}

	function site_shutdown() {
		exit;
	}
	
	function genseed() {
		function rannum() {
			mt_srand((double)microtime()*1000000);
			$num = mt_rand(46,122);
			return($num);
		}
		function genchr() {
			do {
				$num = rannum();
			} while (($num > 57 && $num < 65) || ($num > 90 && $num < 97));

			$char = chr($num);
			return($char);
		}
		$a = genchr();
		$b = genchr();
		$salt = "$a$b";
		return($salt);
	}

	function form_item($desc, $name, $value = "", $required = false,
	                   $size = "", $maxlength = "")
	{
		global $missinginfo;
		
		print "  <tr>\n";
		print "   <td align=\"right\" nowrap=\"nowrap\">";

		if ($required) {
			print "<font color=\"#FF0000\">*</font> ";
		}

		start_font();

		print "<b>";

		if ($missinginfo
		 && (($value == -1 || $value == -2)
		 || ($name == "password" || $name == "password2"))) {
			print "<font color=\"#FF0000\">" . $desc . ":</font>";
		}
		else {
			print $desc . ":";
		}

		print "</b>&nbsp;";

		end_font();

		print "</td>\n";

		print "   <td width=\"60%\">";
		
		start_font();
		
		print "<input type=\"";
		
		if ($name == "password" || $name == "password2") {
			print "password";
		} else {
			print "text";
		}

		print "\" name=\"" . $name . "\"";

		if ($value != -1 && $value != -2 && $value != "") {
			print " value=\""     . $value     . "\"";
		}

		if ($size      != "") { print " size=\""      . $size      . "\""; }
		if ($maxlength != "") { print " maxlength=\"" . $maxlength . "\""; }

		print " />";
		end_font();
		print "</td>\n";
		print "  </tr>\n";
	}

	function file_form($desc, $name, $required = false, $size = "")
	{
		global $missinginfo;
		
		print "  <tr>\n";
		print "   <td align=\"right\" nowrap=\"nowrap\">";

		if ($required) {
			print "<font color=\"#FF0000\">*</font> ";
		}

		start_font();

		print "<b>";

		if ($missinginfo && ($value == -1 || $value == -2)) {
			print "<font color=\"#FF0000\">" . $desc . ":</font>";
		}
		else {
			print $desc . ":";
		}

		print "</b>&nbsp;";

		end_font();

		print "</td>\n";

		print "   <td width=\"60%\">";
		
		start_font();
		
		print "<input type=\"file\" name=\"$name\"";

		if ($size != "") { print " size=\"" . $size . "\""; }

		print " />";
		end_font();
		print "</td>\n";
		print "  </tr>\n";
	}

	function upload_image($filename, $tmpname, $dir, $newname) {
		$namedata = preg_split('/\./', $filename);
		$ext = $namedata[count($namedata) - 1];

		if (!file_exists($dir)) {
			mkdir($dir, 0777);
		}

		$newname = $dir . "/" . $newname . "." . $ext;

		move_uploaded_file($tmpname, $newname);

		@chmod($newname, 0644);
	}
?>
