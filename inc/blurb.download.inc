<?

  // Figure out what browser we're working with
  $useragent = $_SERVER['HTTP_USER_AGENT'];

  // Default vals
  $download_link = "/download/";
  $download_os = "";

  if (strstr($useragent, 'Win')) {
    $download_os = "for Windows";
    $download_link = "/download/windows/";
  } else if (strstr($useragent, "Ubuntu")) {
    // Once the PPA is ready to go public, we'll have a separate Ubuntu page.
    $download_os = "for Ubuntu";
    $download_link = "/download/source/";
  } else if (strstr($useragent, 'CentOS') ||
             strstr($useragent, 'RHEL')) {
    $download_os = "for CentOS / RHEL";
    $download_link = "/download/centos_rhel/";
  } else if (strstr($useragent, "Fedora")) {
    $download_os = "for Fedora Core";
    $download_link = "/download/fedora_core/";
  } else if (strstr($useragent, "Mac")) {
    $download_os = "for Mac OS X";
    $download_link = "/download/mac/";
  } else if (strstr($useragent, 'X11') ||
             strstr($useragent, 'Linux')) {
    $download_os = "Source";
    $download_link = "/download/source/";
  }
?>

<div id="download">
<?php if ($page['section'] == "Home") { ?>
<img src="/shared/img/logo.pidgin.png" alt="[Pidgin Logo]" id="logo" width="107" height="185" />
<?php } ?>
<a id="download_link" href="<? echo $download_link; ?>">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" width="48" height="48" 
/>
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?></span> 
<span class="number" id="os_name"> <? echo $download_os; ?></span></a>
</div>
