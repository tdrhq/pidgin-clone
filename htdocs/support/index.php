<?
  // Update these to match the current page.
  $page['title'] =   "Pidgin help and support";
  $page['section'] = "Help"; // Set to "home2" to enable website test
  $page['description'] = "Starting point for getting help and reporting problems with pidgin";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<div class="box_home">
<div id="left">

<div style="padding:20px 20px;">

<h1>General Pidgin Help</h1>

New to Pidgin?  Try the <a href="http://developer.pidgin.im/wiki/Using%20Pidgin">User Guide</a>.<br/>
Also look at the <a href="http://developer.pidgin.im/wiki/FAQ">Frequently Asked Questions</a>.

<h2>Need to talk to a human?</h2>

Send an email: <a href="http://www.pidgin.im/cgi-bin/mailman/listinfo/support">Pidgin support mailing list</a><br/>
Chat on IRC: <a href="http://widget.mibbit.com/?settings=4102eeafa56ea5c509e3cea7434fca3f&server=irc.freenode.net&channel=%23pidgin" target="_blank">#pidgin on Freenode</a>


</div>

</div>

</div>

<div class="box_home" id="right">
<div id="main">

<h1>I have a problem to report</h1>

<ul>
<li>First, don't panic.</li>
<li><a href="http://developer.pidgin.im/query?status=new&status=pending&col=id&col=summary&order=priority&version=<? echo $pidgin_version; ?>&type=defect">Search</a> to see if it has already been reported.
<li>Otherwise, learn how to <a href="http://developer.pidgin.im/wiki/TipsForBugReports">file a bug report</a>
</ul>

<div id="download_new">
<strong>Pidgin Crashed?</strong><br/><br/>
What to do if a crash occurs:<br/>
<a href="http://developer.pidgin.im/wiki/TipsForBugReports#WindowsUsers">On Windows</a><br/>
<a href="http://developer.pidgin.im/wiki/GetABacktrace">On Linux</a>



</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>