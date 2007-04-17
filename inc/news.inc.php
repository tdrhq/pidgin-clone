<?

class NewsEntry {
	var $id;
	var $title;
	var $date;
	var $paragraphs = array();
}

function load_news_range($offset, $limit) {
	return array_slice(parse_news(), $offset, $limit);
}

function load_news_story($id) {
	$entries = parse_news();
	return array($entries[$id]);
}

function parse_news() {
	$filename = "news.txt";
	$entries = array();

	$fcontents = file($filename);
	$pos = 0;
	$index = 0;
	$entries[$pos] = new NewsEntry();

	while (list($linenum, $line) = each($fcontents)) {
		$index++;

		if (chop($line) == "#") {
			$pos++;
			$entries[$pos] = new NewsEntry();
			$index = 0;
		}
		elseif ($index == 1)
			$entries[$pos]->title = $line;
		elseif ($index == 2)
			$entries[$pos]->date = $line;
		else
			$entries[$pos]->paragraphs[] = $line;
	}

	$entries_out = array();
	$num_entries = count($entries);

	for ($i = 0; $i < $num_entries; $i++) {
		$id = $num_entries - $i;
		$entries[$i]->id = $id;
		$entries_out[$id] = &$entries[$i];
	}

	return $entries_out;
}
?>
