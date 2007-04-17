<?php
	class Users {

		var $user_info  = array();
		var $is_logged_in = false;

		function Users() {
			global $sid, $db;

			if (empty($sid)) {
				$this->is_logged_in = false;
				return;
			}

			$db->query(
				"SELECT * FROM Users WHERE SESSION_ID='" . $sid . "'"
			);

			if ($db->num_rows() > 0) {
				$this->user_info = $db->result_array();

				$this->is_logged_in = true;
			}
		}

		function logged_in() {
			global $sid;

			return $this->is_logged_in;
		}

		function login($username, $password) {
			global $session;

			if ($session->login($username, $password)) {
				$this->is_logged_in = true;
				return true;
			} else {
				return false;
			}
		}

		function logout() {
			global $session;

			$session->destroy();

			$this->is_logged_in = false;
		}

		function user_exists($username) {
			global $db;

			$db->query("SELECT * FROM Users WHERE USERNAME='$username'");

			if ($db->num_rows() > 0) {
				$info = $db->result_array();
				if (!empty($info["USERNAME"])) {
					return true;
				}
			}

			return false;
		}

		function get_user_info_from_id($userid) {
			global $db;

			if ($userid == -1) { return NULL; }

			if ($userid == $this->user_info["USER_ID"]) {
				return $this->user_info;
			}

			$db->query(
				"SELECT * FROM Users WHERE USER_ID=" . $userid
			);

			if ($db->num_rows() > 0) {
				return $db->result_array();
			}

			return NULL;
		}

		function get_user_info_from_username($username) {
			global $db;

			if ($username == "") { return NULL; }

			if ($username == $this->user_info["USERNAME"]) {
				return $this->user_info;
			}

			$db->query(
				"SELECT * FROM Users WHERE USERNAME='" . $username . "'"
			);

			if ($db->num_rows() > 0) {
				return $db->result_array();
			}

			return NULL;
		}

		function get_active_user_username() {
			return $this->user_info["USERNAME"];
		}

		function get_active_user_id() {
			return $this->user_info["USER_ID"];
		}

		function is_active_user_admin() {
			return ($this->user_info["ADMIN"] == "Y");
		}

		function get_active_user_info() {
			return $this->user_info;
		}

		function change_password($username, $newpassword) {
			global $db;

			$seed = genseed();
			$newpass = crypt($newpassword, $seed);

			$db->query(
				"UPDATE Users SET PASSWORD='$newpass'"
			  . " WHERE USERNAME='$username'"
			);
		}

		function register() {
			global $db, $session;

			$bakusername = $GLOBALS["user"];

			$requiredinfo = array("user", "password", "password2");

			$missinginfo = false;

			for ($i = 0; $i < count($requiredinfo); $i++) {
				if (empty($GLOBALS[$requiredinfo[$i]])) {
					$GLOBALS[$requiredinfo[$i]] = -1;
					$missinginfo = true;
				}
			}

			if ($bakusername != "") {
				if ($this->user_exists($bakusername)) {
					$GLOBALS["username"] = -2;
					$missinginfo = true;
				}
			}

			if ($GLOBALS["password"] != $GLOBALS["password2"]) {
				$GLOBALS["password"]  = -1;
				$GLOBALS["password2"] = -1;
			}

			if ($missinginfo) { return false; }

			$seed = genseed();
			$newpass = crypt($GLOBALS["password"], $seed);

			$db->query(
				"INSERT INTO Users VALUES(0, '$GLOBALS[user]', "
			  . "'$newpass', '$GLOBALS[firstname]', '$GLOBALS[lastname]', "
			  . "'$GLOBALS[email]', '$GLOBALS[icquin]', '$GLOBALS[aimname]', "
			  . "'$GLOBALS[yahooname]', '$GLOBALS[jabbername]', "
			  . "'$GLOBALS[msnname]', '$GLOBALS[ggname]', '', 'N')"
			);

			if (!$db->valid_result()) {
				return false;
			}

			return true;
		}
	}
?>
