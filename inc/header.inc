<?

if ($page == NULL || !is_array($page)) {
  $page = array();
}

if (!array_key_exists('title', $page)) {
  $page['title'] = 'Home';
}
if (!array_key_exists('section', $page)) {
  $page['section'] = 'Home';
}

// Define Sections
$sections['Home']                  = "/";
$sections['Download']              = "/download/";
$sections['About']                 = "/about/";
$sections['News']                  = "/news/";
$sections['Support & Development'] = "http://developer.pidgin.im/";

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
		"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
  <title><? echo $page['title']; ?> | Pidgin</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta http-equiv="Content-language" content="en-US" />
  <meta name="copyright" content="Pidgin Contributors" />
  <meta name="author" content="Kevin Stange" />
  <meta name="description" content="The home of Pidgin, Finch, and the libpurple IM client library." />
  <meta name="keywords" content="pidgin, finch, libpurple, purple, IM, instant messaging, internet messaging, AIM, ICQ, XMMP, Jabber, Google Talk, MSN, Yahoo, Gadu-Gadu, IRC, Bonjour, SILC, SIP/SIMPLE, Zephyr, QQ, GroupWise, Sametime, Linux, Windows, GTK+" />
  <link rel="stylesheet" href="/shared/css/main.css" />
</head>
<body>

<div id="header"><img src="/shared/img/logo.text.jpg" alt="Pidgin" />
<ul>
<?
  foreach ($sections as $name => $link) {
    $escaped = htmlspecialchars ($name);
    if ($page['section'] == $name) {
      echo '  <li class="active"><a href="' . $link . '">' . $escaped . "</a></li>\n";
    } else {
      echo '  <li><a href="' . $link . '">' . $escaped . "</a></li>\n";
    }
  }
?>
</ul>
</div>
