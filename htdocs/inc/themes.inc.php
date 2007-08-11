<?php
	class Themes
	{
		function Themes()
		{
		}

		function get_theme_type_id()
		{
			return -1;
		}

		function get_description()
		{
			return "";
		}

		function do_theme_query($sql)
		{
			global $db;

			$db->query($sql);

			if ($db->num_rows() > 0)
			{
				while ($info = $db->result_array())
				{
					$themes[$info["THEME_ID"]] = $info;
				}

				return $themes;
			}

			return NULL;
		}

		function get_theme_count()
		{
			global $db;

			$db->query("SELECT COUNT(*) AS NUM_THEMES FROM Themes " .
			           "WHERE TYPE_ID=" .
			           $this->get_theme_type_id());

			$result = $db->result_array();

			return $result["NUM_THEMES"];
		}

		function get_all_themes($start = 0, $limit = 25)
		{
			$typeid = $this->get_theme_type_id();

			return $this->do_theme_query(
				"SELECT Themes.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Themes, Users" .
				" WHERE Themes.TYPE_ID=$typeid AND" .
				" Themes.USER_ID=Users.USER_ID LIMIT $start,$limit"
			);
		}

		function get_popular_themes($limit = 10)
		{
			$typeid = $this->get_theme_type_id();

			return $this->do_theme_query(
				"SELECT Themes.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Themes, Users" .
				" WHERE Themes.TYPE_ID=$typeid AND " .
				" Themes.USER_ID=Users.USER_ID AND" .
				" Themes.DOWNLOAD_COUNT > 1" .
				" ORDER BY DOWNLOAD_COUNT DESC LIMIT $limit"
			);
		}

		function get_users_themes($user_id)
		{
			return $this->do_theme_query(
				"SELECT * FROM Themes WHERE USER_ID=" . $user_id
			);
		}

		function get_theme_author($theme_id)
		{
			global $db;

			$db->query(
				"SELECT Users.* FROM Users, Themes" .
				" WHERE Themes.THEME_ID=" . $theme_id .
				" AND Users.USER_ID=Themes.USER_ID"
			);

			if ($db->num_rows() > 0)
				return $db->result_array();

			return NULL;
		}

		function get_theme($theme_id)
		{
			global $db;

			$db->query(
				"SELECT Themes.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Themes, Users" .
				" WHERE Themes.THEME_ID=" . $theme_id . " AND " .
				" Themes.USER_ID=Users.USER_ID"
			);

			if ($db->num_rows() > 0)
			{
				$result = $db->result_array();

				while (list($var, $val) = each($result)) {
					$result[$var] = stripslashes($val);
				}

				reset($result);

				return $result;
			}

			return NULL;
		}

		function get_icon($theme_id)
		{
			global $theme_icon_dir;

			$exts = array("png", "gif");

			for ($i = 0; $i < count($exts); $i++)
			{
				if (file_exists($theme_icon_dir . "/$theme_id." . $exts[$i]))
					return $theme_id . "." . $exts[$i];
			}

			return "";
		}

		function add_theme($userid, $name, $version, $url, $homepage,
		                   $filesize, $brief, $description)
		{
			global $db;

			$typeid = $this->get_theme_type_id();

			$name        = addslashes($name);
			$version     = addslashes($version);
			$url         = addslashes($url);
			$homepage    = addslashes($homepage);
			$brief       = addslashes($brief);
			$description = addslashes($description);

			$db->query(
				"INSERT INTO Themes VALUES(0, $typeid, $userid, \"$name\","
			  . " \"$version\", \"$url\", \"$homepage\", $filesize,"
			  . " \"$brief\", \"$description\", NOW(), NOW(), 0, 0)"
			);

			$db->query(
				"SELECT Themes.THEME_ID FROM Themes WHERE"
			  . " NAME=\"$name\" AND VERSION=\"$version\" AND"
			  . " URL=\"$url\""
			);

			$res = $db->result_array();

			return $res["THEME_ID"];
		}

		function update_theme($theme_id, $name, $version, $url, $homepage,
		                      $filesize, $brief, $description)
		{
			global $db;

			$name        = addslashes($name);
			$version     = addslashes($version);
			$url         = addslashes($url);
			$homepage    = addslashes($homepage);
			$brief       = addslashes($brief);
			$description = addslashes($description);

			$db->query(
				"UPDATE Themes SET NAME=\"$name\", VERSION=\"$version\"," .
				" URL=\"$url\", HOMEPAGE=\"$homepage\", FILESIZE=$filesize," .
				" BRIEF=\"$brief\", DESCRIPTION=\"$description\", " .
				" UPDATED=NOW() WHERE THEME_ID=" . $theme_id
			);
		}

		function delete_theme($theme_id)
		{
			global $db;

			$db->query(
				"DELETE FROM Themes WHERE THEME_ID=" . $theme_id
			);
		}

		function update_download_count($theme_id)
		{
			global $db;

			$db->query(
				"UPDATE Themes SET DOWNLOAD_COUNT=DOWNLOAD_COUNT+1" .
				" WHERE THEME_ID=" . $theme_id
			);
		}

		function update_view_count($theme_id)
		{
			global $db;

			$db->query(
				"UPDATE Themes SET VIEW_COUNT=VIEW_COUNT+1" .
				" WHERE THEME_ID=" . $theme_id
			);
		}

		function get_search_count($keywords, $type)
		{
			global $db;

			$typeid = $this->get_theme_type_id();

			$keywords = strtolower(str_replace(" ", "|", $keywords));

			$querystr = "SELECT COUNT(*) FROM Themes WHERE " .
			            "TYPE_ID=$typeid AND LOWER(";

			if ($type == "name")
				$querystr .= "NAME";
			else
				$querystr .= "DESCRIPTION";

			$querystr .= ") REGEXP \"(" . $keywords . ")\"";

			if ($type != "name")
				$querystr .= " OR LOWER(BRIEF) REGEXP \"(" . $keywords . ")\"";

			$db->query($querystr);

			$result = $db->result_array();

			return $result[0]["COUNT(*)"];
		}

		function search_themes($keywords, $type, $start, $limit)
		{
			global $db;

			$typeid = $this->get_theme_type_id();

			$keywords = strtolower(str_replace(" ", "|", $keywords));

			$querystr = "SELECT * FROM Themes WHERE " .
			            "TYPE_ID=$typeid AND LOWER(";

			if ($type == "name")
				$querystr .= "NAME";
			else
				$querystr .= "DESCRIPTION";

			$querystr .= ") REGEXP \"(" . $keywords . ")\"";

			if ($type != "name")
				$querystr .= " OR LOWER(BRIEF) REGEXP \"(" . $keywords . ")\"";

			$querystr .= " LIMIT $start,$limit";

			return $this->do_theme_query($querystr);
		}
	}
?>
