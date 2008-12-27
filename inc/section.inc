<?

if ($sub_sections == NULL || !is_array($sub_sections)) {
  $sub_sections = array();
}

if ($page == NULL || !is_array($page)) {
  $page = array();
}

if (!array_key_exists('subsect', $page)) {
  $page['subsect'] = '';
}

?>
<div class="box_nav">
<ul>
<?
  foreach ($sub_sections as $name => $link) {
    $escaped = htmlspecialchars ($name);
    if ($page['abslink'] && !preg_match("%^[a-z]+:%", $link)) {
      $link = $page['siteroot'] . $link;
    }

    $external = 0;
    // An imperfect way of detecting external URLs.  This works for our current urls.
    if (preg_match("/com/", $link)) {
    	$external = 1;
    }
    
    if ($page['subsect'] == $name) {
      echo '  <li class="active"><a ';
      if ($external > 0) {
         echo 'rel=nofollow ';
      }
      echo 'href="' . $link . '">' . $escaped . "</a></li>\n";
    } else {
      echo '  <li><a ';
      if ($external > 0) {
        echo 'rel=nofollow ';
      }
      echo 'href="' . $link . '">' . $escaped . "</a></li>\n";
    }
  }
?>
</ul>
</div>
