<?

if ($page == NULL || !is_array($page)) {
  $page = array();
}

if (!array_key_exists('abslink', $page)) {
  $page['abslink'] = FALSE;
}
if (!array_key_exists('siteroot', $page)) {
  $page['siteroot'] = "http://www.pidgin.im";
}
if (!array_key_exists('title', $page)) {
  $page['title'] = 'Home';
}
if (!array_key_exists('description', $page)) {
  $page['description'] = 'The home of Pidgin, Finch, and the libpurple IM client library.';
}
if (!array_key_exists('section', $page)) {
  $page['section'] = 'Home';
}

// Define Sections
$sections['Home']                  = "/";
$sections['Download']              = "/download/";
$sections['About']                 = "/about/";
$sections['News']                  = "http://planet.pidgin.im/";
$sections['Support & Development'] = "http://developer.pidgin.im/";

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
		"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
  <title><? echo $page['title']; ?></title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta http-equiv="Content-language" content="en-US" />
  <meta name="copyright"   content="Pidgin Contributors" />
  <meta name="author"      content="Pidgin Contributors" />
  <meta name="designer"    content="Blend Interactive; http://blendinteractive.com" />
  <meta name="generator"   content="Kevin Stange ;)" />
  <meta name="description" content="<? echo $page['description'] ?>" />
  <link rel="alternate" href="http://planet.pidgin.im/rss20.xml" type="application/rss+xml" title="Pidgin News" />
  <link rel="shortcut icon" href="/shared/img/favicon.png" type="image/png" />
  <link rel="stylesheet" href="/shared/css/main.css" type="text/css" />
  <!-- Correct rendering bugs from Microsoft Internet Explorer -->
  <!--[if lt IE 7]>
    <script type="text/javascript">
      IE7_PNG_SUFFIX = ".png";
    </script>
    <script src="/shared/js/ie7/ie7-standard-p.js" type="text/javascript"></script>
    <link rel="stylesheet" href="/shared/css/ie6.css" type="text/css" />
  <![endif]-->
  <!--[if IE 7]>
    <script defer src="/shared/js/ie7-hacks.js" type="text/javascript"></script>
    <link rel="stylesheet" href="/shared/css/ie7.css" type="text/css" />
  <![endif]-->

  <?php if($page['section'] == 'Home2') { ?>
    <?php include("multibox.header.inc"); ?>
  <?php } ?>

</head>
<body>

<div id="header">
<a href="/"><img src="/shared/img/logo.text.jpg" alt="Pidgin" /></a>
<ul>
<?
  foreach ($sections as $name => $link) {
    $escaped = htmlspecialchars ($name);
    if ($page['abslink'] && !preg_match("%^[a-z]+:%", $link)) {
      $link = $page['siteroot'] . $link;
    }
    if ($page['section'] == $name) {
      echo '  <li class="active"><a href="' . $link . '">' . $escaped . "</a></li>\n";
    } else {
      echo '  <li><a href="' . $link . '">' . $escaped . "</a></li>\n";
    }
  }
?>
</ul>
</div>
