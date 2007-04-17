<?

    	header("HTTP/1.1 301 Moved Permanently");
        header('Location: http://developer.pidgin.im/wiki/BuildingWinPidgin');
	exit();

	
	$page->title = 'Windows Development';
	require('../inc/template.inc.php');
?>

<h1>Windows Gaim Build Instructions</h1>

<h2>Set up your build environment</h2>

<h3>The easy way</h3>

<ol>
<li>
<p>
Install the <a href="http://cygwin.com">Cygwin</a> Bash shell. Make sure to select Unix file mode during setup.  Also make sure you install bash, bzip2, coreutils, gawk, grep, gzip, make, patch, sed, subversion, tar, unzip, and wget (several of these are selected by default).
</p>
</li>
<li>
<p>
Download the <a href="http://gaim-extprefs.sourceforge.net/bef.shtml">WinGaim Build Environment Fetcher</a> script.  Follow the instructions on the website to fetch and install most of the needed build dependencies.  The script can fetch Gaim's source code from either Subversion or a source distribution; you will be prompted to choose which you'd like to use.<br/>
Note that this script is sometimes out of date, in which case you will need to perform some of the manual steps in addition.
</p>
</li>
<li>
<p>Skip the &quot;The manual way,&quot; &quot;Install Gaim's build dependencies,&quot; and &quot;Get the Gaim source code&quot; sections and go straight to &quot;<a href="#buildgaim">Build Gaim</a>&quot;.
</p>
</li>
</ol>

<h3>The manual way</h3>

<ol>
<li>Install the <a href="http://cygwin.com">Cygwin</a> Bash shell. Make sure to select Unix file mode during setup.  Also make sure you install bash, bzip2, coreutils, gawk, grep, gzip, make, patch, sed, subversion, tar, unzip, and wget (several of these are selected by default).</li>
<li>
<p>
Install the MinGW "current" packages from the <a href="http://www.mingw.org">MinGW site</a>.<br/>
The easiest way to install these is to use the MinGW Installer linked to from the main site.<br/>
Specifically, you will need gcc-core 3.4.2, binutils 2.15.91, mingw-runtime 3.9 and win32api 3.6 (or newer).<br/>
You will need to set MinGW's bin directory before Cygwin's in your PATH.<br/>
For Example (You can add the following to <code>~/.bash_login</code>):
</p>

<pre>
<code>export PATH=/cygdrive/c/MinGW/bin:$PATH</code>
</pre>

<p>You do not want to use any of Cygwin's build tools, with one exception; you need to use Cygwin's make utility. If typing <code>which make</code> produces MinGW's make utility, then just rename the <code>make.exe</code> in the MinGW bin directory to something else so that cygwin's make utility will be used instead. This shouldn't be a problem with recent versions of MinGW.</p>
</li>

<? /*
<li>
<p>
Download the latest version of the <code>Windows API</code> from the MinGW <a href="http://www.mingw.org/download.shtml">download page</a> (at least <a href="http://prdownloads.sourceforge.net/mingw/w32api-3.3.tar.gz?download">3.3</a>).  Extract this into your MinGW directory structure (<code>tar -xvzf /path/to/w32api-3.3.tar.gz -C /cygdrive/c/MinGW</code>) to upgrade the installed API.
</p>
</li>
*/ ?>
<li>
The following instructions were written under the assumption that you checkout Gaim from the Subversion repository to <code>~/gaim</code> and that you install all of Gaim's build dependencies under <code>~/win32-dev</code> (the point being that the gaim and win32-dev directories should be on the same level).
</li>
</ol>

<h2>Install Gaim's build dependencies</h2>
<dl>
<dt>GTK+</dt>
<dd>Gaim depends on GTK+ <?php print $current_win32_gtk_version; ?>.  For your convenience, we have included all of GTK's dependencies in one tarball. Extract <a href="http://prdownloads.sourceforge.net/gaim/gtk-dev-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.tar.gz">gtk-dev-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.tar.gz</a> from within <code>~/win32-dev</code>. If once built you wish to run Gaim from the win32-install-dir dir, you will need to make sure that you have installed the GTK+ runtime, and make sure that its bin dir is in your PATH. Visit the <a href="http://www.gtk.org">GTK+ website</a> for official binary and source releases.</dd>

<dt>Libxml2</td>
<dd>Download and extract <a href="libxml2-2.6.24.tar.gz">libxml2-2.6.24.tar.gz</a> to <code>win32-dev</code>.</dd>

<dt>Perl 5.8</dt>
<dd>Install Perl 5.8 for Windows (I use <a href="http://www.activestate.com/Products/Language_Distributions/">ActivePerl</a>), to <code>C:\Perl</code>. If you install Perl anywhere else, you will need to change the <code>PERL</code> and <code>EXTUTILS</code> variables in <code>gaim/plugins/perl/common/Makefile.mingw</code>.  You will also need to install <a href="perl582.tar.gz">perl582.tar.gz</a> under <code>~/win32-dev</code> (Containing altered headers and import lib).</dd>

<dt>Tcl 8.4.5</dt>
<dd>Download and extract <a href="tcl-8.4.5.tar.gz">tcl-8.4.5.tar.gz</a> to <code>win32-dev</code>.</dd>

<dt>GtkSpell / Aspell</dt>
<dd>Download the following development packages for GtkSpell and Aspell, and extract them under <code>win32-dev</code>:
<ul>
  <li><a href="http://ftp.gnu.org/gnu/aspell/w32/aspell-dev-0-50-3-3.zip">aspell-dev-0-50-3-3.zip</a></li>
  <li><a href="gtkspell-2.0.6.tar.gz">gtkspell-2.0.6.tar.gz</a></li>
</ul>
</dd>

<dt>Mozilla NSS</dt>
<dd>Download and unzip the following under <code>~/win32-dev</code> (make sure you <code>chmod 755</code> all the unzipped binaries):
<ul>
<li>
<a href="ftp://ftp.mozilla.org/pub/mozilla.org/security/nss/releases/NSS_3_11_4_RTM/msvc6.0/WIN954.0_OPT.OBJ/nss-3.11.4.zip">Network Security Services (NSS)</a>
</li>
<li>
<a href="ftp://ftp.mozilla.org/pub/mozilla.org/nspr/releases/v4.6.4/msvc6.0/WIN954.0_OPT.OBJ/nspr-4.6.4.zip">Netscape Portable Runtime (NSPR)</a>
</li>
</ul>
</dd>

<dt>SILC Toolkit</dt>
<dd>Download and extract <a href="silc-toolkit-1.0.2.tar.gz">silc-toolkit-1.0.2.tar.gz</a> to <code>win32-dev</code>.</dd>

<dt>Meanwhile</dt>
<dd>Download and extract <a href="http://prdownloads.sourceforge.net/meanwhile/meanwhile-1.0.2-win32.zip?download">meanwhile-1.0.2-win32.zip</a> to <code>win32-dev</code>.</dd>

</dl>

<h2>Get the Gaim source code</h2>
<ul>
  <li>Sources are in <a href="http://gaim.sourceforge.net/downloads.php">Subversion</a></li>
  <li><a href="http://prdownloads.sourceforge.net/gaim/gaim-<?php print $current_win32_version; ?>.tar.gz">Windows Gaim v<?php print $current_win32_version; ?> sources</a></li>
</ul>

<h2><a name="buildgaim">Build Gaim</a></h2>
<pre>
<code>$ cd ~/gaim
$ make -f Makefile.mingw install</code>
</pre>
<p>Now just wait and let your compiler do its thing.  When finished, Gaim will be in <code>~/gaim/win32-install-dir</code></p>

<h2>Build Gaim Installer</h2>
If you want to build the Gaim installer, do the following (skip to the <code>make</code> command below if you chose to use the WinGaim Build Environment Fetcher):
<ol>
<li>
Grab and install <a href="http://nsis.sourceforge.net/Download">NSIS</a>. Include NSIS to your PATH.<br/>
For info on the NSIS installer, visit the <a href="http://nsis.sourceforge.net">NSIS website</a>.
</li>
<li>
Grab 
<a href="http://prdownloads.sourceforge.net/gaim/gtk-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>-installer.tar.gz?download">gtk-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>-installer.tar.gz</a> 
and extract it to <code>~/</code> (or whichever directory you have the gaim dir in). From within the new gtk_installer directory, type ". build.sh" (this builds the GTK+ runtime installer, which the Gaim installer will include).
</li>
<li>
Grab <a href="gaim-inst-deps.tar.gz">gaim-inst-deps.tar.gz</a> and extract under <code>~/win32-dev</code>.
</li>
</ol>
Now do the following:
<pre>
<code>$ cd ~/gaim
$ make -f Makefile.mingw installer</code>
</pre>

<h2>Debugging</h2>
<p>
There is a quite good <b>Just In Time</b> debugger for MinGW: <a href="http://jrfonseca.planetaclix.pt/projects/gnu-win32/software/drmingw/index.html">drmingw</a>. You can download it <a href="drmingw.exe">here</a>.
</p>
