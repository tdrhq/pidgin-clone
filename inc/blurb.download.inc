<?php
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.downloaddetect.inc");
?>

<div id="download">
<?php if ($page['section'] == "Home") { ?>
<img src="/shared/img/logo.pidgin.png" alt="[Pidgin Logo]" id="logo" width="107" height="185" />
<?php } ?>
<a id="download_link" href="<? echo $download_link; ?>"
<?php if (strlen($download_goal) > 0) { ?>
 onClick="javascript:ConversionCount();"
<?php } ?>
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" width="48" height="48" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?></span> 
<span class="number" id="os_name"> <? echo $download_os; ?></span></a>
<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/langdetect.inc"); 
if (strlen($language) > 0 && $language != "English") {
?>
<span class="number"><? echo $language ?></span>
<? } ?>
</div>
