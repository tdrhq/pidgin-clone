<?
	$page->title = 'FAQ';
	require('inc/template.inc.php');

	require_once('inc/faq-functions.inc.php');
?>

<h1>Frequently Asked Questions</h1>
<p><i>Frequently asked questions specific to Windows have their own <a href="/win32">FAQ</a>.</i></p>
<p><i>For those users testing <a href="http://sourceforge.net/project/showfiles.php?group_id=235&package_id=253&release_id=479631">2.0.0beta6</a>, we have an updated <a href="faq2.php">FAQ</a>.</i></p>
<?
	/* The Q&A begins here */
	$faq = file("./faq.txt");
	$faq_lines = count($faq);
	$sections = array();
	
	$qa = 1;
	$in_section = 0;
	$in_question = 0;

	/* Loop through once to get all the questions. */
	for ($i = 0; $i < $faq_lines; $i++)
	{
		$line = $faq[$i];

		if ($line == "" || $line == "\n" || substr($line, 0,3) == "A:\t")
		{
			if ($in_question)
			{
				print "</a></li>\n";
				$in_question = 0;
			}

			continue;
		}
		elseif (substr($line, 0, 3) == "Q:\t")
		{
			if ($in_question)
			{
				print "</a></li>\n";
				$in_question = 0;
			}

			$question = substr($line, 3);
			$question = preg_replace(
				"/(<\/?(br|code|pre|div)( ?\/)?>|[\r\n\t])/", " ", $question);

			print " <li><a href=\"#q" . $qa . "\">";
			print $question;

			$in_question = 1;

			$qa++;
		}
		elseif (substr($line, 0, 9) == "!SECTION\t")
		{
			if ($in_question)
			{
				print "</a></li>\n";
				$in_question = 0;
			}

			if ($in_section)
				print "</ul>\n";

			print "<b>" . trim(substr($line, 9)) . "</b>\n";
			print "<ul>\n";

			$in_section = 1;
		}
		elseif (substr($line, 0, 9) == "!COMMENT\t")
		{
		}
		elseif ($in_question)
		{
			print preg_replace(
				"/(<\/?(br|code|pre|div)( ?\/)?>|[\r\n\t])/", " ", $line);
		}
	}

	if ($in_question == 1)
	{
		print "</a></li>\n";
	}

	if ($in_section == 1)
	{
		print "</ul>\n";
	}

	/* Now we'll loop through again to print out the questions and answers. */
	$qa = 1;
	$in_section = 0;
	$in_question = 0;
	$in_answer = 0;
	$anchor = "";

	for ($i = 0; $i < $faq_lines; $i++)
	{
		$line = $faq[$i];

		if (substr($line, 0, 3) == "Q:\t")
		{
			check_in_qa();

			start_q();
			print substr($line, 3);

			$qa++;

			$in_question = 1;
		}
		elseif (substr($line, 0, 3) == "A:\t")
		{
			check_in_qa();

			start_a();
			print substr($line, 3);

			$in_answer = 1;
		}
		elseif (substr($line, 0, 9) == "!SECTION\t")
		{
			check_in_qa();

			if ($in_section == 1)
			{
				print "</dl>\n";
			}

			$name = trim(substr($line, 9));

			print "<a name=\"" . build_section_name($name) . "\"></a>\n";
			print "<h2>$name</h2>";
			print "<dl class=\"faq\">\n";

			$in_section = 1;
		}
		elseif (substr($line, 0, 8) == "!ANCHOR\t")
		{
			$anchor = trim(substr($line, 8));
		}
		elseif (substr($line, 0, 9) == "!COMMENT\t")
		{
		}
		elseif ($in_question || $in_answer)
		{
			print $line;
		}
	}

	check_in_qa();

	if ($in_section == 1)
	{
		print "</dl>\n";
	}

?>
