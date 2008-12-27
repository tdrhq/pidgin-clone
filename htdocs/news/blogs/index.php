<?
  // Update these to match the current page.
  $page['title'] =   "Blogs | News";
  $page['section'] = "News";
  $page['subsect'] = "Blogs";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.news.inc"); ?>

<div class="box_right">
<div id="main">

<h1>Blogs</h1>

<p>While our developers' blogs are syndicated on our news page, they can also
be found at their respective sources, which may permit comments or offer
additional historical information for the more adventurous or bored.</p>

<ul>
  <li><a href="http://theflamingbanker.blogspot.com/">John Bailey</a></li>
  <li><a href="/~seanegan/blog/">Sean Egan</a></li>
  <li><a href="/~elb/blog/">Ethan Blanton</a></li>
  <li><a href="http://gary-kramlich.livejournal.com/">Gary Kramlich</a></li>
  <li><a href="http://coderich.net/category/open-source/pidgin/">Richard Laager</a></li>
  <li><a href="http://www.schierer.org/~luke/log/category/personal/gaim">Luke Schierer</a></li>
  <li><a href="/~kstange/blog/">Kevin Stange</a></li>
  <li><a href="http://faceprint.com/?tags=pidgin,gaim">Nathan Walp</a></li>
</ul>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
