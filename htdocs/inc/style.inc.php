<?php
	class Style {
		var $img_dir;
		var $sidebar_bg;
	}

	class NormalStyle extends Style {
		var $img_dir = "/images";
		var $sidebar_bg = "#6486C1";
	}

	class HalloweenStyle extends Style {
		var $img_dir = "/images/halloween";
		var $sidebar_bg = "#DD8304";
	}

	class ThanksgivingStyle extends Style {
		var $img_dir = "/images/thanksgiving";
		var $sidebar_bg = "#A76F39";
	}

	class StyleFactory {
		var $styles = array(
			"NORMAL" => "NormalStyle",
			"halloween" => "HalloweenStyle",
			"thanksgiving" => "ThanksgivingStyle"
		);

		function load_style($style) {
			if (!$style) {
				/*
				if (date("F") == "October") {
					$style = "halloween";
				}
				elseif (date("F") == "November") {
					$style = "thanksgiving";
				}
				else {
				*/
					$style = "NORMAL";
				//}
			}

			if (!isset($this->styles[$style]))
				$style = "NORMAL";

			return new $this->styles[$style];
		}
	}

	$style_factory = new StyleFactory();

	//$style = $style_factory->load_style($layout);
	$style = $style_factory->load_style("");
?>
