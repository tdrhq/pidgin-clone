<?php
	class Session {
		var $sid = "";          /* Session ID   */

		/********************************************************************
		 * function Session()
		 * 
		 * The Session() constructor
		 ********************************************************************/
		function Session() {
		}

		function startSession() {
			global $sid, $user, $db;
			global $HTTP_COOKIE_VARS, $HTTP_GET_VARS, $HTTP_POST_VARS;
			global $PHP_SELF;

			if ((!isset($sid)) || (empty($sid))) { return; }

			$this->sid = $sid = urldecode($sid);

			$db->query(
				"SELECT * FROM Users WHERE SESSION_ID='" . $sid . "'"
			);

			if ($db->num_rows() == 0) {
				/*
				 * Oops, a cookie is set but there is no session info.
				 * You know what we have to do... KILL THE COOKIE!
				 */
				setcookie("sid", "", 0);
				unset($GLOBALS["sid"]);
				unset($HTTP_COOKIE_VARS["sid"]);
				unset($this->sid);
			}
		}
		
		function login($user, $password) {
			global $db, $users;
			global $HTTP_USER_AGENT, $PHP_SELF, $HTTP_COOKIE_VARS;

			$missinginfo = false;

			$GLOBALS["user_missing"] = false;
			$GLOBALS["password_missing"] = false;

			if ($user != "") {
				if (!$users->user_exists($user)) {
					$GLOBALS["user_missing"] = -1;
					$GLOBALS["password_missing"] = -1;
					$missinginfo = true;
				}
			}
			else {
				if ($user == "") {
					$GLOBALS["user_missing"] = -1;
					$missinginfo = true;
				}
				if ($password == "") {
					$GLOBALS["password_missing"] = -1;
					$missinginfo = true;
				}
			}
			
			if ($missinginfo) { return false; }

			if ($users->logged_in()) {
				return true;
			}

			$db->query(
				"SELECT * FROM Users WHERE USERNAME='".$user."'"
			);

			if ($db->num_rows() > 0) {
				$info = $db->result_array();
				$salt = substr($info["PASSWORD"], 0, 2);
				$newpass = crypt($password, $salt);

				if ($newpass != $info["PASSWORD"]) {
					return false;
				}

				$this->sid = $this->generate_get_sid();

				$db->query(
					"UPDATE Users SET SESSION_ID='" . $this->sid . "'"
				  . " WHERE USER_ID=" . $info["USER_ID"]
				);

				$HTTP_COOKIE_VARS["sid"] = $this->sid;
				
				$expdate = time() + 63072000;
				setcookie("sid", $this->sid, $expdate);

				header("Location: " . $this->localUrl($PHP_SELF) . "\n");
				exit;
			}

			return false;

		}
		
		/********************************************************************
		 * function generate_get_sid()
		 * 
		 * Generates a Session ID based on the date/time and IP address.
		 ********************************************************************/
		function generate_get_sid() {
			return "s" . md5(uniqid(rand()));
		}

		/********************************************************************
		 * function get_sid()
		 * 
		 * Returns the session ID in a pib_sessionid=... way.
		 ********************************************************************/
		function get_sid() {
			return "sid=" . urlencode($this->sid);
		}

		/********************************************************************
		 * function sid_value()
		 * 
		 * Returns the session ID.
		 ********************************************************************/
		function sid_value() {
			return urlencode($this->sid);
		}
		
		function localUrl($url) {
			return $url;
			
/*
			if (empty($this->sid)) {
				return $url . "?";
			} else {
				return $url . "?" . $this->get_sid();
			}
*/
		}
		
		function hidden_sid() {
			return "";

/*
			if (empty($this->sid)) {
				return "";
			} else {
				return "<input type=\"hidden\" name=\"sid\""
				     . " value=\"" . $this->get_sid() . "\" />";
			}
*/
		}
		
		/********************************************************************
		 * function destroy()
		 * 
		 * Destroys the Session and removes all data and variables
		 * associated with it.
		 ********************************************************************/
		function destroy() {
			global $db;
			global $PHP_SELF;

			$db->query(
				"UPDATE Users SET SESSION_ID='' WHERE SESSION_ID='" .
				$this->sid . "'"
			);

			/* We don't care if this is a valid result or not.      */
			/* If it doesn't exist already, it's already deleted :) */

			unset($this->sid);

//			header("Location: $PHP_SELF");
//			exit;
		}
	}
?>
