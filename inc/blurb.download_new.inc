<?php
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.downloaddetect.inc");
?>

<div class="prominent_box">
<form method="get" action="<? echo $download_link; ?>">
<div>
<button type="submit" id="download_link" 
<?php if (strlen($download_goal) > 0) { ?>
 onclick="javascript:ConversionCount();"
<?php } ?>
>
Download Pidgin
</button>
</div>
</form>

<br />


Version <? echo $pidgin_version; ?> <? echo $download_os; ?>

<br />

<div class="linkbar">
  <a href="/download/">Other Versions</a> |
  <a href="http://developer.pidgin.im/wiki/ChangeLog">ChangeLog</a> |
  <a href="http://developer.pidgin.im/wiki/FAQ" title="Frequently Asked Questions">FAQ</a>
</div>

</div>
