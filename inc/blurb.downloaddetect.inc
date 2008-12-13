<?

  // Figure out what browser we're working with
  $useragent = $_SERVER['HTTP_USER_AGENT'];

  // Default vals
  $download_link = "/download/";
  $download_os = "";
  $download_goal = "";

  if (strstr($useragent, 'Win')) {
    $download_os = "for Windows";
    $download_link = "http://downloads.sourceforge.net/pidgin/pidgin-$pidgin_version.exe";
    $download_goal = "/goals/downloads/win";
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

<!-- Begin google website optimizer tracking code -->

<script type="text/javascript">
<!--
// run conversion tracking script
function ConversionCount()
{
//_uacct = 'UA-6596609-1';
pageTracker._trackPageview('<? echo $download_goal; ?>');
secondTracker._trackPageview("/1527532502/goal");
return true;
}
// -->
</script>

<!-- End google website optimizer tracking code -->

