<?
  // This file exists to act as a second version of index.php for website experiment tests.
  // This needs to always exist, and either redirect to index.php or be the home of an experiment


  // Update these to match the current page.
  $page['title'] =   "Pidgin, the universal IM client";
  $page['section'] = "Home";
  $page['description'] = "Pidgin is a free and easy to use IM client used by millions. Connect to AIM, MSN, Yahoo, and other IM networks all at once.";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>


<div id="content">

<div class="box_full">
<div id="main">
<div></div>


<div id="sidebar">
<!--<a href="/shared/img/blist.jpg" id="screen0" class="multibox"> -->
  <img src="/shared/img/blist.jpg" alt="Sample IM buddy list" />
<!--  </a> -->
</div>


<div id="main_content">

<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download_new.inc");
?>
<br />

<h1>IM <strong>all</strong> your friends in one place</h1>



<?php if($_GET['v'] == "2") { ?> 
<p>Pidgin is an easy to use and free IM client used by millions.</p>

<div style="font-size:125%">
<ul>

<li>
<strong>One single program, all of your networks</strong><br />
Connects to AIM, Yahoo, MSN and more.<br />
<a href="/shared/img/im_networks.jpg" id="protocols" target="_new">View supported IM networks</a>
</li>
<li>
<strong>No ads</strong><br />
100% free and actively supported by the open source community
</li>
<li>
<strong>Simple yet customizable</strong><br />
An intuitive interface that can be extended by plugins.<br />
<!--<a href="/shared/img/screens/account-options.jpg" id="screen4" class="multibox">View screenshots</a>-->

</li>
</ul>

</div>

<p class="more"><a href="/about/"><img
src="/shared/img/button.learn-more.png" alt="Learn More" width="128"
height="32"/></a></p>


<?php } else { ?>


<p>Pidgin is an easy to use and free IM client used by millions. Connect 
to AIM, MSN, Yahoo, and more IM networks all at once.</p>

<p>Supported IM networks:</p>

<ul class="condensed">
  <li>AIM</li>
  <li>Bonjour</li>
  <li>Gadu-Gadu</li>
  <li>Google Talk</li>
  <li>Groupwise</li>
  <li>ICQ</li>
  <li>IRC</li>
  <li>MSN</li>
  <li>MySpaceIM</li>
  <li>QQ</li>
  <li>SILC</li>
  <li>SIMPLE</li>
  <li>Sametime</li>
  <li>XMPP</li>
  <li>Yahoo!</li>
  <li>Zephyr</li>
</ul>

<p class="more"><a href="/about/"><img 
src="/shared/img/button.learn-more.png" alt="Learn More" width="128" 
height="32"/></a>
</p>

<? } ?>

</div>
</div>
</div>

<?php /*
<div style="display:none">

<div class="multiboxD protocols">Pidgin supports these IM networks.  Additional protocols are available through plugins.</div>

<div class="multiboxD screen0">One buddy list, all of your IM networks</div>

<div class="multiboxD screen4">Manage all your IM accounts from one menu</div>

<a href="/shared/img/screens/blist-status.jpg" id="screen2" class="multibox"></a>
<div class="multiboxD screen2">Set your status across all IM networks with a single click</div>

<a href="/shared/img/screens/conversation.jpg" id="screen3" class="multibox"></a>
<div class="multiboxD screen3">Easy to use chat/IM windows</div>

<a href="/shared/img/screens/plugins.jpg" id="screen6" class="multibox"></a>
<div class="multiboxD screen6">Many plugins are available to help tweak every aspect of Pidgin</div>


</div>
*/ ?>


<!-- p class="more" id="lowblurb">We are aware of the problems connecting to ICQ with Pidgin
2.4.2 and older. We hope to have a new release and packages out soon to fix
this. In the meantime, if you compile Pidgin from source you can use this
<a href="pidgin-2.4.2-icq6.patch">patch</a> to fix it.</p -->

<? /* Avoid outputting this stuff yet.
<!-- These are not ready to go yet -->
 <div class="box_full">

<div id="newsbrief">
Test
</div>

<div id="blogbrief">
Test
</div>

</div> */ ?>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>