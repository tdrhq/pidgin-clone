<?php
/*
 * $Id: database.inc.php 3629 2002-09-26 10:21:44Z chipx86 $
 *
 * Portal In A Bottle
 * Database routines
 * Copyright (C) 1999 Portal Web Design. All rights reserved.
 *
 * A class implementing database routines with support for
 * many types of databases.
 * For use with the Portal In A Bottle toolkit.
 *
 * Part of the Portal In A Bottle base utilities.
 */
	
	class Database {
		var $dbclass;

		var $host;
		var $username;
		var $password;
		var $database;

		var $result;
		
		/********************************************************************
		 * function Database($db_server_type, $host, $username, $password,
		 *                   $database)
		 *   $db_server_type - Database type, defaults to "mysql"
		 *   $host           - Host where the database server resides
		 *   $username       - Username for the database
		 *   $password       - Password for the database
		 *   $database       - Database name
		 * 
		 * Constructor
		 ********************************************************************/
		function Database($dbserver_type = "mysql", $host = "localhost", $username = "", $password = "", $database = "") {
			$dbserver_type = strtolower($dbserver_type);
//			if (empty($host)) { fatal_error("Host required"); }

			if (empty($username)) {
				fatal_error("Username required");
			}

			if ($dbserver_type == "mysql") {
				$this->dbclass = new Database_MySQL();
			}
			else {
				fatal_error("This database format is not supported!");
			}
			
			$this->dbclass->host = $host;
			$this->dbclass->username = $username;
			$this->dbclass->password = $password;
			$this->dbclass->database = $database;
		}

		/********************************************************************
		 * function connect()
		 * 
		 * Connects to the database
		 ********************************************************************/
		function connect() {
			$this->dbclass->connect();
		}

		/********************************************************************
		 * function disconnect()
		 * 
		 * Closes the connection to the database
		 ********************************************************************/
		function disconnect() {
			$this->dbclass->disconnect();
		}

		/********************************************************************
		 * function set_database($db)
		 *   $db     - Database name
		 * 
		 * Sets which database to use
		 ********************************************************************/
		function set_database($db) {
			$this->dbclass->database = $db;
		}
		
		/********************************************************************
		 * function get_database()
		 * 
		 * Returns the name of the current database.
		 ********************************************************************/
		function get_database() {
			return($this->dbclass->database);
		}
		
		/********************************************************************
		 * function query($query)
		 *   $query  - the SQL query
		 * 
		 * Queries the database
		 ********************************************************************/
		function query($query) {
			$this->free_result();
//			print "SQL Call = {$query}<br />\n";
			$this->dbclass->query($query);
		}

		/********************************************************************
		 * function row_exists($rowname)
		 *   $rowname - The name of the row
		 * 
		 * Returns TRUE if the specified row exists. FALSE otherwise.
		 ********************************************************************/
		function row_exists($rowname) {
			$this->free_result();
			return $this->row_exists($rowname);
		}

		/********************************************************************
		 * function result_array()
		 * 
		 * Returns the result in array form
		 ********************************************************************/
		function result_array() {
			return($this->dbclass->result_array());
		}

		/********************************************************************
		 * function valid_result()
		 * 
		 * Returns TRUE if the result is not NULL, FALSE otherwise.
		 ********************************************************************/
		function valid_result() {
			if (empty($this->dbclass->result)) {
				return FALSE;
			}

			return TRUE;
		}

		/********************************************************************
		 * function num_rows()
		 * 
		 * Returns the number of rows in the previous query.
		 ********************************************************************/
		function num_rows() {
			return($this->dbclass->num_rows());
		}

		/********************************************************************
		 * function free_result()
		 * 
		 * Frees the $result.
		 ********************************************************************/
		function free_result() {
			$this->dbclass->free_result();
		}

		function get_insert_id() {
			return $this->dbclass->get_insert_id();
		}
	}


/*********************************************************************/


	/********************************************************************
	 * class Database_MySQL extends Database
	 * 
	 * MySQL database implementation
	 ********************************************************************/
	class Database_MySQL extends Database {
		
		function Database_MySQL() {}
		
		/********************************************************************
		 * function connect()
		 * 
		 * Connects to the database
		 ********************************************************************/
		function connect() {
			mysql_pconnect($this->host, $this->username, $this->password)
				or fatal_error(mysql_error());
		}

		/********************************************************************
		 * function disconnect()
		 * 
		 * Closes the connection to the database
		 ********************************************************************/
		function disconnect() {
			/*
			 * We want a persistant connection. This would defeat the purpose.
			 */
//			mysql_close();
		}
		
		/********************************************************************
		 * function query($query)
		 *   $query  - SQL query
		 * 
		 * Queries the database
		 ********************************************************************/
		function query($query) {
			$this->result = mysql_db_query($this->database, $query)
				or fatal_error(mysql_error());			
		}

		/********************************************************************
		 * function row_exists($rowname)
		 *   $rowname - The name of the row
		 *   $key     - The key to use in the COUNT() (optional)
		 *   $misc    - Misc. stuff 
		 * 
		 * Returns TRUE if the specified row exists. FALSE otherwise.
		 ********************************************************************/
		function row_exists($rowname, $key = "*", $misc = "") {
			$this->query("SELECT COUNT($key) FROM $this->database");
		}
		
		/********************************************************************
		 * function result_array()
		 * 
		 * Returns the result in an array form
		 ********************************************************************/
		function result_array() {
			return(mysql_fetch_array($this->result));
		}

		/********************************************************************
		 * function num_rows()
		 * 
		 * Returns the number of rows in the previous query.
		 ********************************************************************/
		function num_rows() {
			return(mysql_num_rows($this->result));
		}

		/********************************************************************
		 * function free_result()
		 * 
		 * Frees the $result.
		 ********************************************************************/
		function free_result() {
			//mysql_free_result($this->result);
			unset($this->result);
		}

		function get_insert_id() {
			return mysql_insert_id();
		}
	}
?>
