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

<p>While most of our developers' blogs are syndicated on our news page, they
also can be viewed directly at their respective sources.</p>

<ul>
  <li><a href="/~seanegan/blog/">Sean Egan</a></li>
  <li><a href="/~elb/blog/">Ethan Blanton</a></li>
  <li><a href="http://coderich.net/">Richard Laager</a></li>
  <li><a href="http://www.schierer.org/~luke/log">Luke Schierer</a></li>
  <li><a href="/~kstange/blog/">Kevin Stange</a> (Webmaster)</li>
  <li><a href="http://faceprint.com/">Nathan Walp</a></li>
</ul>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
