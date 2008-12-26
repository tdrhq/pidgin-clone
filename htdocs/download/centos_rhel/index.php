<?
  // Update these to match the current page.
  $page['title'] =   "CentOS / RHEL | Download";
  $page['section'] = "Download";
  $page['subsect'] = "CentOS / RHEL";

  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/header.inc");
  include($_SERVER['DOCUMENT_ROOT'] . "/../inc/version.inc");
?>

<div id="content">

<? include($_SERVER['DOCUMENT_ROOT'] . "/../inc/section.download.inc"); ?>

<div class="box_right">
<div id="main">

<h1><? echo $page['subsect']; ?></h1>

<div class="box_download">

<p><a href="http://rpm.pidgin.im/centos/pidgin.repo"
onClick="javascript:pageTracker._trackPageview('/goals/downloads/centos_rhel');"
>
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" />
Download Pidgin<br />
<span class="number"><? echo $pidgin_version; ?> for CentOS / RHEL</span>
</a></p>

<p>To install Pidgin on CentOS 4 or 5, or RHEL 5, save the pidgin.repo file
from above in /etc/yum.repos.d/</p>

<p>You can then use &quot;yum install pidgin&quot;, or your favorite package
manager, to install Pidgin</p>

<p>To install Pidgin on RHEL 4, add the following line to
/etc/sysconfig/rhn/sources:
<br />yum pidgin http://rpm.pidgin.im/rhel/4/$ARCH</p>

<p>You will also need to install the GPG signing key manually:<br />
rpm --import http://rpm.pidgin.im/RPM-GPG-KEY-pidgin</p>

<p>You can then use &quot;up2date -u pidgin&quot; to install Pidgin.</p>

</div>

</div>
</div>

</div>

<?php include($_SERVER['DOCUMENT_ROOT'] . "/../inc/footer.inc"); ?>
