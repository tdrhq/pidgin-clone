<?
  header('HTTP/1.1 404 Not Found');

  // Update these to match the current page.
  $page['title'] =   "404 Not Found";
  $page['section'] = "";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
?>

<div id="content">

<div class="box_full">
<div id="main">

<h1>404 Not Found</h1>

<p>Sorry, the page you tried to reach isn't here!  Please try using other
links on the site to find what you're looking for.  If you're sure this
page should work, please submit a <a
href="http://developer.pidgin.im/simpleticket">new ticket</a> to let us
know about the problem.</p>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
