<?php
	class Table
	{
		var $headers;
		var $header_aligns;
		var $rows;
		var $alignments;
		var $valignments;
		var $rowinfo;
		var $widths = 0;
		var $alt_row_colors;
		var $border;
		var $header_color = "#f7c871";

		function Table($headers, $alt_row_colors = false, $border = false)
		{
			$this->headers = $headers;
			$this->alt_row_colors = $alt_row_colors;
			$this->border = $border;

			if ($headers != 0)
			{
				for ($i = 0; $i < count($headers); $i++)
				{
					$this->header_aligns[$i] = "";
				}
			}
		}

		function set_header_align($col, $align)
		{
			$this->header_aligns[$col] = $align;
		}

		function set_widths($widths)
		{
			$this->widths = $widths;
		}

		function add_row($values, $info = 0)
		{
			$this->rows[]    = $values;
			$this->rowinfo[] = $info;
		}

		function set_alignment($col, $align)
		{
			$this->alignments[$col] = $align;
		}

		function set_valignment($col, $align)
		{
			$this->valignments[$col] = $align;
		}

		function output_html()
		{
			$row_size = count($this->rows);

			$oddrow = true;

			if ($this->border)
			{
				print "<table border=\"0\" cellspacing=\"0\"";
				print " cellpadding=\"1\" width=\"100%\">\n";
				print " <tr>\n";
				print "  <td bgcolor=\"#000000\">\n";
			}

			print "<table border=\"0\" cellspacing=\"0\"";
			print " cellpadding=\"3\" width=\"100%\">\n";

			if (is_array($this->headers))
			{
				$header_size = count ($this->headers);

				print " <tr>\n";

				for ($i = 0; $i < $header_size; $i++)
				{
					print "  <td bgcolor=\"" . $this->header_color . "\"";
					print " nowrap=\"nowrap\"";

					if ($this->header_aligns[$i] != "")
						print " align=\"" . $this->header_aligns[$i] . "\"";

					if ($this->widths != 0)
						print " width=\"" . $this->widths[$i] . "\"";

					print ">";
					start_font();
					print "<b>" . $this->headers[$i] . "</b>";
					end_font();
					print "</td>\n";
				}
			}

			for ($i = 0; $i < $row_size; $i++)
			{
				print " <tr>\n";

				$col_count = count($this->rows[$i]);

				for ($j = 0; $j < $col_count; $j++)
				{
					print "  <td";

					if ($this->alt_row_colors == true)
					{
						if ($oddrow)
							print " bgcolor=\"#F0F0F0\"";
						else
							print " bgcolor=\"#FFFFFF\"";
					}

					if ($i == 0 && $this->headers == 0)
					{
						if ($this->widths != 0)
							print " width=\"" . $this->widths[$i] . "\"";
					}

					if ($this->alignments != 0 && $this->alignments[$j])
					{
						print " align=\"" . $this->alignments[$j] . "\"";
					}

					if ($this->valignments != 0 && $this->valignments[$j])
					{
						print " valign=\"" . $this->valignments[$j] . "\"";
					}

					if ($this->rowinfo[$i] != 0 && $this->rowinfo[$i][$j] != 0)
					{
						while (list($var, $val) = each($this->rowinfo[$i][$j]))
						{
							print " $var=\"$val\"";
						}
					}

					print ">";

					start_font();
					print $this->rows[$i][$j];
					end_font();

					print "</td>";
				}

				$oddrow = !$oddrow;

				print " </tr>\n";
			}

			if ($this->border)
			{
				print "</td></tr></table>\n";
			}

			print "</table>\n";
		}
	}
?>
