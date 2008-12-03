
<div id="download">
<?php if ($page['section'] == "Home") { ?>
<img src="/shared/img/logo.pidgin.png" alt="[Pidgin Logo]" id="logo" width="107" height="185" />
<?php } ?>
<a id="download_link" href="/download/">
<img src="/shared/img/button.download.png" alt="[Down Arrow]" id="downbutton" width="48" height="48" 
/>
Free Download<br />
<span class="number"><? echo $pidgin_version; ?></span>
<span class="number" id="os_name" style="display: none;"></span></a>

<script type="text/javascript"><!--
if (navigator.platform.indexOf("Win") != -1) {
        document.getElementById("download_link").href = "/download/windows";
        document.getElementById("os_name").innerHTML = "for Windows";
        document.getElementById("os_name").style.display = "inline";
} else if (navigator.userAgent.indexOf("Ubuntu") != -1) {
// Once the PPA is ready to go public, we'll have a separate Ubuntu page.
        document.getElementById("download_link").href = "/download/source";
        document.getElementById("os_name").innerHTML = "for Ubuntu";
        document.getElementById("os_name").style.display = "inline";
} else if (navigator.userAgent.indexOf("CentOS") != -1 ||
           navigator.userAgent.indexOf("RHEL") != -1) {
        document.getElementById("download_link").href = "/download/centos_rhel/";
        document.getElementById("os_name").innerHTML = "for CentOS / RHEL";
        document.getElementById("os_name").style.display = "inline";
} else if (navigator.userAgent.indexOf("Fedora") != -1) {
        document.getElementById("download_link").href = "/download/fedora_core/";
        document.getElementById("os_name").innerHTML = "for Fedora Core";
        document.getElementById("os_name").style.display = "inline";
} else if (navigator.platform.indexOf("Mac") != -1) {
        document.getElementById("download_link").href = "/download/mac/";
        document.getElementById("os_name").innerHTML = "for Mac OS X";
        document.getElementById("os_name").style.display = "inline";
} else if (navigator.platform.indexOf("X11") != -1 ||
           navigator.platform.indexOf("Linux")) {
        document.getElementById("download_link").href = "/download/source";
        document.getElementById("os_name").innerHTML = "Source";
        document.getElementById("os_name").style.display = "inline";
}
// --></script>
</div>

