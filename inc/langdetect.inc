<?

// Language detection code

$languages["af"]="Afrikaans";
$languages["ar"]="Arabic";
$languages["be"]="Belarusian Latin";
$languages["bg"]="Bulgarian";
$languages["bn"]="Bengali";
$languages["bs"]="Bosnian";
$languages["ca"]="Catalan";
$languages["cs"]="Czech";
$languages["da"]="Danish";
$languages["de"]="German";
$languages["dz"]="Dzongkha";
$languages["el"]="Greek";
$languages["en"]="English";
$languages["en_US"]="American English";
$languages["en_AU"]="Australian English";
$languages["en_CA"]="Canadian English";
$languages["en_GB"]="British English";
$languages["eo"]="Esperanto";
$languages["es"]="Spanish";
$languages["et"]="Estonian";
$languages["eu"]="Euskera(Basque)";
$languages["fa"]="Persian";
$languages["fi"]="Finnish";
$languages["fr"]="French";
$languages["ga"]="Irish";
$languages["gl"]="Galician";
$languages["gu"]="Gujarati";
$languages["he"]="Hebrew";
$languages["hi"]="Hindi";
$languages["hu"]="Hungarian";
$languages["id"]="Indonesian";
$languages["it"]="Italian";
$languages["ja"]="Japanese";
$languages["ka"]="Georgian";
$languages["kn"]="Kannada";
$languages["ko"]="Korean";
$languages["ku"]="Kurdish";
$languages["lo"]="Lao";
$languages["lt"]="Lithuanian";
$languages["mk"]="Macedonian";
$languages["mn"]="Mongolian";
$languages["nb"]="Bokmal Norwegian";
$languages["ne"]="Nepali";
$languages["nl"]="Dutch, Flemish";
$languages["nn"]="Norwegian Nynorsk";
$languages["oc"]="Occitan";
$languages["pa"]="Punjabi";
$languages["pl"]="Polish";
$languages["pt"]="Portuguese";
$languages["pt_BR"]="Portuguese-Brazil";
$languages["ps"]="Pashto";
$languages["ro"]="Romanian";
$languages["ru"]="Russian";
$languages["sk"]="Slovak";
$languages["sl"]="Slovenian";
$languages["sq"]="Albanian";
$languages["sr"]="Serbian";
$languages["si"]="Sinhala";
$languages["sv"]="Swedish";
$languages["ta"]="Tamil";
$languages["te"]="Telugu";
$languages["th"]="Thai";
$languages["tr"]="Turkish";
$languages["ur"]="Urdu";
$languages["vi"]="Vietnamese";
$languages["zh"]="Chinese";
$languages["zh_CN"]="Simplified Chinese";
$languages["zh_HK"]="Hong Kong Chinese";
$languages["zh_TW"]="Traditional Chinese";

if (empty($_SERVER["HTTP_ACCEPT_LANGUAGE"])) {
  $language_code = "";
  $language = "";
} else {
  $language_code = substr($_SERVER['HTTP_ACCEPT_LANGUAGE'], 0, 2);
  $language = $languages[$language_code];
}
?>

