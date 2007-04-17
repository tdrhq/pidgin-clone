<?php
	require('inc/news.inc.php');

	function left($str, $num)
	{
		return substr($str, 0, $num);
	}

	function rss_parse_date($date)
	{
		$date = str_replace("st,", "", $date);
		$date = str_replace("nd,", "", $date);
		$date = str_replace("rd,", "", $date);
		$date = str_replace("th,", "", $date);
		$date = str_replace(" - ", " ", $date);

		$timestamp = strtotime($date);

		return date("r", $timestamp);
	}

	function rss_header($title, $description, $url)
	{
		print "<?xml version=\"1.0\"?>\n\n";
		print "<rss version=\"0.91\">\n";
		print " <channel>\n";
		print "  <title>$title</title>\n";
		print "  <description>$description</description>\n";
		print "  <link>$url</link>\n";
	}

	function rss_footer()
	{
		print " </channel>\n";
		print "</rss>\n";
	}

	function rss_start_item($title, $date, $link)
	{
		$title = chop($title);
		$date = chop($date);

		$title = htmlentities($title);

		$date = rss_parse_date($date);

		print "  <item>\n";
		print "   <title>$title</title>\n";
		print "   <pubDate>$date</pubDate>\n";
		print "   <link>$link</link>\n";
	}

	function rss_end_item()
	{
		print "  </item>\n";
	}

	function rss_description($text)
	{
		$text = chop($text);
		$text = htmlentities($text);

		print "   <description>\n";
		print "    $text\n";
		print "   </description>\n";
	}

	if (!isset($_SERVER['PATH_INFO']))
	{
		print "PATH_INFO was not passed.";
		exit();
	}

	if ($_SERVER['PATH_INFO'] == "/news")
	{
		header("Content-type: text/xml");

		rss_header("Pidgin news", "Pidgin news",
		           "http://pidgin.im/index.php");

		if (isset($_GET['start'])) $start = $_GET['start']; else $start = 0;
		if (isset($_GET['limit'])) $limit = $_GET['limit']; else $limit = 10;

		$entries = load_news_range($start, $limit);

		foreach ($entries as $entry) {
			rss_start_item($entry->title, $entry->date,
				"http://pidgin.im/index.php?id=".$entry->id);

			$desc = '';
			foreach ($entry->paragraphs as $paragraph) {
				$desc .= $paragraph.' ';
			}
			rss_description($desc);
			rss_end_item();
		}

		rss_footer();
	}
	else
	{
		print "RSS information for ".$_SERVER['PATH_INFO']." is not provided.";
	}

?>
