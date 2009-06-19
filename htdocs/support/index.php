<?
  // Update these to match the current page.
  $page['title'] =   "Pidgin help and support";
  $page['section'] = "Help"; // Set to "home2" to enable website test
  $page['description'] = "Starting point for getting help with the Pidgin chat client.";

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

Send an email: <a href="http://pidgin.im/cgi-bin/mailman/listinfo/support">Pidgin support mailing list</a><br/>
Chat on IRC: <a onclick="javascript:this.target='_blank'; pageTracker._trackPageview('/external/webchat.freenode.net');" href="http://webchat.freenode.net/?channels=pidgin">#pidgin on Freenode</a>

</div>

</div>

</div>

<div class="box_home" id="right">
<div id="main">

<h1>I have a problem to report</h1>

<div class="prominent_box">
<strong>Common issues</strong><br/>
<a href="http://www.celticwolf.com/useful-information/faqs/26-pidgin-yahoo">Pidgin cannot connect to Yahoo!</a><br/>
</div>

<ul>
<li>First, don't panic.</li>
<li><a href="http://developer.pidgin.im/query?status=new&amp;status=pending&amp;col=id&amp;col=summary&amp;order=priority&amp;version=<? echo $pidgin_version; ?>&amp;type=defect">Search</a> to see if it has already been reported.</li>
<li>Otherwise, learn how to <a href="http://developer.pidgin.im/wiki/TipsForBugReports">file a bug report</a></li>
</ul>

<div>
<br/>
<strong>Pidgin Crashed?</strong><br/><br/>
What to do if a crash occurs:<br/>
<a href="http://developer.pidgin.im/wiki/TipsForBugReports#WindowsUsers">On Windows</a><br/>
<a href="http://developer.pidgin.im/wiki/GetABacktrace">On Linux</a>



</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
