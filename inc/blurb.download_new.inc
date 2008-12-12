<?php
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.downloaddetect.inc");
?>

<div id="download_new">
<img src="/shared/img/bigdownload.top.gif" alt="" />
<div>
<a id="download_link" href="<? echo $download_link; ?>"
<?php if (strlen($download_goal) > 0) { ?>
 onClick="javascript:pageTracker._trackPageview('<? echo $download_goal; ?>');ConversionCount();"
<?php } ?>
>
<strong>Download Pidgin <? echo $download_os; ?> </strong> <br />


version <? echo $pidgin_version; ?>
</div>

<img src="/shared/img/bigdownload.bottom.gif" alt="" />

</div>
