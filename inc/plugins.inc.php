<?php
	class Plugins
	{
		function Plugins()
		{
		}

		function do_plugin_query($sql)
		{
			global $db;

			$db->query($sql);

			if ($db->num_rows() > 0)
			{
				while ($info = $db->result_array())
				{
					$plugins[$info["PLUGIN_ID"]] = $info;
				}

				return $plugins;
			}
			
			return NULL;
		}

		function get_plugin_count()
		{
			global $db;

			$db->query("SELECT COUNT(*) FROM Plugins");

			$result = $db->result_array();
			
			return $result[0]["COUNT(*)"];
		}

		function get_all_plugins($start = 0, $limit = 25)
		{
			return $this->do_plugin_query(
				"SELECT Plugins.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Plugins, Users" .
				" WHERE Plugins.USER_ID=Users.USER_ID LIMIT $start,$limit"
			);
		}
		
		function get_popular_plugins($limit = 10)
		{
			return $this->do_plugin_query(
				"SELECT Plugins.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Plugins, Users" .
				" WHERE Plugins.USER_ID=Users.USER_ID AND" .
				" Plugins.DOWNLOAD_COUNT > 1" .
				" ORDER BY DOWNLOAD_COUNT DESC LIMIT $limit"
			);
		}

		function get_users_plugins($user_id)
		{
			return $this->do_plugin_query(
				"SELECT * FROM Plugins WHERE USER_ID=" . $user_id
			);
		}

		function get_plugin_author($plugin_id)
		{
			global $db;

			$db->query(
				"SELECT Users.* FROM Users, Plugins" .
				" WHERE Plugins.PLUGIN_ID=" . $plugin_id .
				" AND Users.USER_ID=Plugins.PLUGIN_ID"
			);

			if ($db->num_rows() > 0)
				return $db->result_array();

			return NULL;
		}

		function get_plugin($plugin_id)
		{
			global $db;

			$db->query(
				"SELECT Plugins.*, Users.USER_ID, Users.USERNAME," .
				" Users.FIRSTNAME, Users.LASTNAME FROM Plugins, Users" .
				" WHERE Plugins.PLUGIN_ID=" . $plugin_id . " AND " .
				" Plugins.USER_ID=Users.USER_ID"
			);

			if ($db->num_rows() > 0)
				return $db->result_array();

			return NULL;
		}

		function add_plugin($userid, $name, $version, $url, $homepage,
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
				"INSERT INTO Plugins VALUES(0, $userid, \"$name\","
			  . " \"$version\", \"$url\", \"$homepage\", $filesize,"
			  . " \"$brief\", \"$description\", NOW(), NOW(), 0, 0)"
			);
		}

		function update_plugin($plugin_id, $name, $version, $url, $homepage,
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
				"UPDATE Plugins SET NAME=\"$name\", VERSION=\"$version\"," .
				" URL=\"$url\", HOMEPAGE=\"$homepage\", FILESIZE=$filesize," .
				" BRIEF=\"$brief\", DESCRIPTION=\"$description\", " .
				" UPDATED=NOW() WHERE PLUGIN_ID=" . $plugin_id
			);
		}

		function delete_plugin($plugin_id)
		{
			global $db;

			$db->query(
				"DELETE FROM Plugins WHERE PLUGIN_ID=" . $plugin_id
			);
		}

		function update_download_count($plugin_id)
		{
			global $db;

			$db->query(
				"UPDATE Plugins SET DOWNLOAD_COUNT=DOWNLOAD_COUNT+1" .
				" WHERE PLUGIN_ID=" . $plugin_id
			);
		}

		function update_view_count($plugin_id)
		{
			global $db;

			$db->query(
				"UPDATE Plugins SET VIEW_COUNT=VIEW_COUNT+1" .
				" WHERE PLUGIN_ID=" . $plugin_id
			);
		}

		function get_search_count($keywords, $type)
		{
			global $db;

			$keywords = strtolower(str_replace(" ", "|", $keywords));

			$querystr = "SELECT COUNT(*) FROM Plugins WHERE LOWER(";

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

		function search_plugins($keywords, $type, $start, $limit)
		{
			global $db;

			$keywords = strtolower(str_replace(" ", "|", $keywords));

			$querystr = "SELECT * FROM Plugins WHERE LOWER(";

			if ($type == "name")
				$querystr .= "NAME";
			else
				$querystr .= "DESCRIPTION";

			$querystr .= ") REGEXP \"(" . $keywords . ")\"";
			
			if ($type != "name")
				$querystr .= " OR LOWER(BRIEF) REGEXP \"(" . $keywords . ")\"";

			$querystr .= " LIMIT $start,$limit";

			return $this->do_plugin_query($querystr);
		}
	}
?>
