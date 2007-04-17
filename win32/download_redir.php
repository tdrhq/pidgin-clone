<?php

/* Build the map of packageID to URL */
$dl_map = array();
$dl_map['aspell_core'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-0-50-3-3-Setup.exe';
$dl_map['lang_br'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-br-0.50-2-3.exe';
$dl_map['lang_ca'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-ca-0.50-2-3.exe';
$dl_map['lang_cs'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-cs-0.50-2-3.exe';
$dl_map['lang_cy'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-cy-0.50-3-3.exe';
$dl_map['lang_da'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-da-0.50-2-3.exe';
$dl_map['lang_de'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-de-0.50-2-3.exe';
$dl_map['lang_el'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-el-0.50-3-3.exe';
$dl_map['lang_en'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-en-0.50-2-3.exe';
$dl_map['lang_eo'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-eo-0.50-2-3.exe';
$dl_map['lang_es'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-es-0.50-2-3.exe';
$dl_map['lang_fo'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-fo-0.50-2-3.exe';
$dl_map['lang_fr'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-fr-0.50-3-3.exe';
$dl_map['lang_it'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-it-0.50-2-3.exe';
$dl_map['lang_nl'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-nl-0.50-2-3.exe';
$dl_map['lang_no'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-no-0.50-2-3.exe';
$dl_map['lang_pl'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-pl-0.50-2-3.exe';
$dl_map['lang_pt'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-pt-0.50-2-3.exe';
$dl_map['lang_ro'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-ro-0.50-2-3.exe';
$dl_map['lang_ru'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-ru-0.50-2-3.exe';
$dl_map['lang_sk'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-sk-0.50-2-3.exe';
$dl_map['lang_sv'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-sv-0.50-2-3.exe';
$dl_map['lang_uk'] = 'http://ftp.gnu.org/gnu/aspell/w32/Aspell-uk-0.50-3-3.exe';

/* Retrieve the version - we don't do anything with it right now */
if (isset($_GET['version'])) {
    $version = $_GET['version'];
}

if (isset($_GET['dl_pkg']) && isset($dl_map[$_GET['dl_pkg']])) {
    header("HTTP/1.1 302 Moved Temporarily");
    header('Location: ' . $dl_map[$_GET['dl_pkg']]);
} else {
    header("HTTP/1.1 400 Bad Request");
    header("Status: 400 Bad Request");
}

exit();
?>
