<!-- Begin Google Website Optimizer tracking code -->
<script>
_udn=".pidgin.im";
function utmx_section(){}function utmx(){}
(function(){var k='1527532502',d=document,l=d.location,c=d.cookie;function f(n){
if(c){var i=c.indexOf(n+'=');if(i>-1){var j=c.indexOf(';',i);return c.substring(i+n.
length+1,j<0?c.length:j)}}}var x=f('__utmx'),xx=f('__utmxx'),h=l.hash;
d.write('<sc'+'ript src="'+
'http'+(l.protocol=='https:'?'s://ssl':'://www')+'.google-analytics.com'
+'/siteopt.js?v=1&utmxkey='+k+'&utmx='+(x?x:'')+'&utmxx='+(xx?xx:'')+'&utmxtime='
+new Date().valueOf()+(h?'&utmxhash='+escape(h.substr(1)):'')+
'" type="text/javascript" charset="utf-8"></sc'+'ript>')})();
</script><script>utmx("url",'A/B');</script>
<!-- End Google Website Optimizer tracking code -->


<?
  // Update these to match the current page.
  $page['title'] =   "Pidgin, the universal IM client";
  $page['section'] = "Home";
  $page['description'] = "Pidgin is an easy to use and free IM client used by millions. Connect to AIM, Yahoo, MSN and other IM networks all at once.";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<div class="box_home">
<div id="left">

<div id="version"><img src="/shared/img/logo.text-big.gif" alt="Pidgin" />
<span class="number"><? echo $pidgin_version; ?></span>
</div>

<?
include($_SERVER['DOCUMENT_ROOT'] . "/../inc/blurb.download.inc");
?>

<p class="linkbar">
  <a href="http://developer.pidgin.im/wiki/ChangeLog">ChangeLog</a> |
  <a href="http://developer.pidgin.im/wiki/FAQ" title="Frequently Asked Questions">FAQ</a>
</p>

</div>

</div>

<div class="box_home" id="right">
<div id="main">

<h1>IM all your friends in one place</h1>

<p>Pidgin is an easy to use and free IM client used by millions. Connect to AIM, MSN, Yahoo, and more IM networks all at once.</p>

<p>Supported IM networks:</p>

<ul class="condensed">
  <li>AIM</li>
  <li>Bonjour</li>
  <li>Gadu-Gadu</li>
  <li>Google Talk</li>
  <li>Groupwise</li>
  <li>ICQ</li>
</ul>
<ul class="condensed">
  <li>IRC</li>
  <li>MSN</li>
  <li>MySpaceIM</li>
  <li>QQ</li>
  <li>SILC</li>
</ul>
<ul class="condensed">
  <li>SIMPLE</li>
  <li>Sametime</li>
  <li>XMPP</li>
  <li>Yahoo!</li>
  <li>Zephyr</li>
</ul>

<p class="more"><a href="/about/"><img src="/shared/img/button.learn-more.png" alt="Learn More" width="128" height="32"/></a></p>

</div>
</div>

<!-- This will pull from somewhere else at some point -->
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