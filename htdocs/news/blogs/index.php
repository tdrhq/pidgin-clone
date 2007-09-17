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
also can be viewed directly at their respective sources.

<ul>
  <li><a href="/~seanegan/blog/">Sean Egan</a> (Lead Developer)</li>
  <li><a href="/~elb/blog/">Ethan Blanton</a></li>
  <li><a href="http://coderich.net/">Richard Laager</li>
  <li><a href="http://www.schierer.org/~luke/log">Luke Schierer</li>
  <li><a href="/~kstange/blog/">Kevin Stange (Webmaster)</li>
  <li><a href="http://faceprint.com/">Nathan Walp</a>
</ul>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
