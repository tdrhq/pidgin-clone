<?
	function build_section_name($section)
	{
		$newstr = str_replace("/", "_", $section);
		$newstr = str_replace(" ", "_", $newstr);
		$newstr = str_replace("(", "_", $newstr);
		$newstr = str_replace(")", "_", $newstr);
		$newstr = str_replace("!", "_", $newstr);

		return $newstr;
	}

	function start_q()
	{
		global $qa, $anchor;

		print " <dt class=\"faq-question\" id=\"q" . $qa . "\">\n";

		if ($anchor != "")
		{
			print "<a name=\"" . $anchor . "\"></a>\n";
			$anchor = "";
		}
	}

	function end_q() {
		print " </dt>\n";
	}

	function start_a()
	{
		print " <dd class=\"faq-answer\">\n";
	}

	function end_a()
	{
		print "</dd>\n";
	}

	function check_in_qa()
	{
		global $in_question;
		global $in_answer;

		if ($in_question)
		{
			end_q();
			$in_question = 0;
		}
		elseif ($in_answer)
		{
			end_a();
			$in_answer = 0;
		}
	}
?>
