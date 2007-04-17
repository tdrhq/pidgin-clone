<?
	$page->title = 'News';
	require('inc/template.inc.php');
	require('inc/news.inc.php');
?>

<div id="rss">
 Get the news in RSS
 <a href="/rss.php/news"><img src="/images/feed-icon.png" border="0" alt="XML" /></a>
</div>
<?php

	if (isset($_GET['start'])) $start = $_GET['start']; else $start = 0;
	if (isset($_GET['limit'])) $limit = $_GET['limit']; else $limit = 5;
	if (isset($_GET['id'])) $id = $_GET['id']; else $id = 0;

	if ($id == 0)
		$entries = load_news_range($start, $limit);
	else
		$entries = load_news_story($id);


	foreach ($entries as $entry) {
		print "<br />\n";
		print "<h2 class=\"news\">$entry->title</h2>\n";
		print "<div class=\"newsdate\"><a href=\"index.php?id=".$entry->id."\">$entry->date</a></div>\n";
		foreach ($entry->paragraphs as $paragraph) {
			print "<p>" . $paragraph . "</p>\n";
		}
	}

	if ($id != 0) {
		print "<a href=\"index.php\">&#171;&nbsp;Current News</a>";
	}
	else {
		print "<table border=\"0\" width=\"100%\">\n";
		print " <tr>\n";
		print "  <td style=\"border: 0;\">";

		if ($start > 0) {
			print "<a href=\"index.php?start=";
			print (($start - $limit > 0) ? ($start - $limit) : 0);
			print "&amp;limit=$limit\">&#171;&nbsp;Newer News</a>";
		}
		else {
			print "&nbsp;";
		}

		print "</td>\n";
		print "  <td align=\"right\" style=\"border: 0;\">";

		print "<a href=\"index.php?start=";
		print ($start + $limit);
		print "&amp;limit=$limit\">";
		print "Older News&nbsp;&#187;</a>";

		print "</td>\n";
		print " </tr>\n";
		print "</table>\n";
	}

?>

