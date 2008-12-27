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
    if ($page['subsect'] == $name) {
      echo '  <li class="active"><a href="' . $link . '">' . $escaped . "</a></li>\n";
    } else {
      echo '  <li><a href="' . $link . '">' . $escaped . "</a></li>\n";
    }
  }
?>
</ul>
</div>
