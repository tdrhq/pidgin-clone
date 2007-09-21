<?
	$page->title = 'Windows Pidgin';
	require('../inc/template.inc.php');
	$current_win32_version = '2.0.0beta7';
	$current_win32_gtk_version = '2.10.11';
	$current_win32_gtk_revision = 'b';
?>

<p><span style="color: #FF0000;">**Note**</span> 1.5.0 does <b>NOT</b> work with MSN and will crash trying to log in.  Use <a href="http://sourceforge.net/project/showfiles.php?group_id=235&package_id=230234">2.0.0beta3.1 or newer</a> to avoid this problem.</p>

<h1>Windows Pidgin</h1>

<p>Grab the latest <a href="http://prdownloads.sourceforge.net/pidgin/pidgin-<?php print $current_win32_version; ?>.exe">Windows build (v<?php print $current_win32_version; ?>)</a>, or if you already have GTK+ v<?php print $current_win32_gtk_version; ?> rev <?php print $current_win32_gtk_revision; ?> installed grab <a href="http://prdownloads.sourceforge.net/pidgin/pidgin-<?php print $current_win32_version; ?>-no-gtk.exe">Windows Pidgin without GTK+</a>.</p>

<p><span style="color: #FF0000;">**Note**</span> GTK+ versions newer than 2.6.10 do <b>NOT</b> work on Windows 95/98/ME.  Use <a href="http://downloads.sourceforge.net/pidgin/gtk-runtime-2.6.10-rev-a.exe">the standalone 2.6.10 rev a</a> installer and download <a href="http://prdownloads.sourceforge.net/pidgin/pidgin-<?php print $current_win32_version; ?>-no-gtk.exe">Windows Pidgin without GTK+</a>.</p>

<p>For the latest Windows Pidgin specific changes, see <a href="ChangeLog.win32">ChangeLog.win32</a>. It sometimes includes information on GTK+ changes/fixes affecting Windows Pidgin.</p>

<p>Windows Pidgin users/developers are encouraged to congregate in #pidgin-win32 on irc.freenode.net.</p>

<p>
<b>NOTE:</b> Those who previously used the <code>GAIMHOME</code> and <code>GAIMLANG</code> environmental variables to define the settings path and language, will need to use <code>PURPLEHOME</code> and <code>PIDGINLANG</code> as of version 2.0.0beta7.
</p>

<h3>Contents</h3>
<ul>
<li><a href="#faq">Windows Pidgin FAQ</a></li>
<li><a href="#bugs">Major Windows Pidgin Bugs &amp; Workarounds</a></li>
<li><a href="#wingtk">Standalone GTK+ Installer</a></li>
<li><a href="#autoinst">Unattended Installations</a></li>
<li><a href="#spelling">Spell-checking</a></li>
<li><a href="#perl">Perl Plugin Support</a></li>
<li><a href="#tcl">TCL Plugin Support</a></li>
<li><a href="#debug">Windows Pidgin Crash Reporting - Howto</a></li>
<li><a href="http://developer.pidgin.im/wiki/BuildingWinPidgin">Windows Build Instructions</a></li>
<li><a href="#portable">Running Pidgin from a USB Drive</a></li>
</ul>

<h2 id="faq">Windows Pidgin FAQ</h2> (These are being migrated to a wiki: <a href="http://developer.pidgin.im/wiki/Using%20Pidgin#WindowsSpecific">here</a>. Feel free to assist in the migration.)
<ul>
<li>
<b>How does Window Pidgin determine which language to use?</b> - <a href="http://developer.pidgin.im/wiki/Using%20Pidgin#HowdoesPidginforWindowsdeterminewhichlanguagetouse">See wiki</a>
</li>

<li>
<b>Where are my Gaim settings and data (including logs) saved?</b> - As of 0.69, <code>shfolder.dll</code> is used to determine the location for a user's app data.  On Windows ME this will be something like <code>C:\WINDOWS\Profiles\username</code> and on Win 2k/XP <code>C:\Documents and Settings\username\Application Data</code>. This can be overridden by setting the <code>GAIMHOME</code> environmental variable (as of v1.1.0).  Logs are stored in this directory in a subdirectory called &quot;logs.&quot;
</li>

<li>
<b>How do I make the fonts bigger? (GTK+ default font setting)</b> - To change your default font and/or size, edit your <code>gtkrc</code> file (located in your <code>GTK\2.0\etc\gtk-2.0</code> dir), with the font name and size of your choosing. For example:
<pre><code>gtk-font-name="sans 8"</code></pre>
Note: If you are using the WIMP theme, this setting is overridden; you can either change Windows standard font sizes, which will likely make other applications have rather large text, or you can use the <a href="http://gaim-extprefs.sourceforge.net/ep.shtml">Extended Preferences</a> plugin.  The plugin offers more finely-grained control such as you would be able to achieve with a gtkrc file and a non-WIMP GTK+ theme.
</li>

<li>
<b>Native Windows look and feel</b> - GTK is themable, which means that you can change colors, fonts and how the actual widgets are drawn. The Gaim installer, gives you the choice of installing one of 3 GTK+ themes.  If you choose GTK-Wimp, Gaim will look and feel like a normal Windows application (including on XP).  To switch back to no theme or to change the theme just run the Gaim installer again and pick a different theme. You can visit the gtk-wimp site at <a href="http://gtk-wimp.sf.net">http://gtk-wimp.sf.net</a>
</li>

<li>
<b>Smiley Themes</b> - You can drag and drop the <a href="http://sourceforge.net/tracker/?atid=746976&amp;group_id=235&amp;func=browse">Smiley Themes</a> into Pidgin.  Simply download a theme to your machine (e.g. to your Desktop), bring up the Smiley theme list in the Preferences dialog, then drag and drop the saved theme into the Smiley themes list.  You do not need to keep the file you downloaded.
</li>

<li>
<b>How do I configure Norton Internet Security for Pidgin?</b> - Unfortunately, Norton Internet Security doesn't correctly respect specifying "Permit All" for a program, and the following additional settings are necessary for Pidgin to function properly.  Symantec has be notified of the problem, but attempts to get it resolved have been fruitless.
<ol>
  <li>Select Options -&gt; Norton Internet Security</li>
  <li>Select the Firewall tab</li>
  <li>Under HTTP port list add the following ports:
    <ol style="list-style: lower-alpha;">
      <li>443  -&gt; MSN (ssl)</li>
      <li>1863 -&gt; MSN</li>
      <li>5050 -&gt; Yahoo</li>
      <li>5190 -&gt; AIM/ICQ</li>
      <li>5222 -&gt; Jabber (including Google Talk)</li>
      <li>6667 -&gt; IRC</li>
    </ol>
  </li>
</ol>
</li>

<li>
<b>How do I run multiple instances of Pidgin?</b> - <a href="http://developer.pidgin.im/wiki/Using%20Pidgin#HowdoIrunmultipleinstancesofPidginonWindows">See wiki</a>
</li>

<li>
<b>How do I upgrade Pidgin?</b> - Close any existing Pidgin instances.  Install the new version of Pidgin.  Start Pidgin.  The installer should detect your existing installation and default to that directory, so you should not need to change the installation directory.
</li>

<li>
<b>Why do I get a &quot;No disk in drive&quot; (or similar) message?</b> - **This has been fixed in the GTK+ 2.10.6 installer.** There is a bug in one of the components of the GTK+ runtime (gettext).  As a result, this component attempts to locate a configuration file on the I: drive.  If you have an I: drive and it is a removable media drive such as a ZIP drive, CD/DVD-ROM drive, or flash card reader, you will see the an error message stating that there is no disk in the drive or asking you to insert a disk into the drive.  If you can change your drive letters so that you have no I: (possible on Windows 2000 or XP), unplug the device if it is a USB device, or insert a disk or card into the drive before starting Pidgin, this should silence the message.  This error message can safely be ignored without any ill effects.
</li>

</ul>

<h2 id="bugs">Major Windows Pidgin Bugs and Workarounds</h2>
<ul>
<li>
<b>TCL Loader Plugin + Cygwin = crash</b> - Starting with 0.75, Pidgin for Windows comes with a tcl plugin loader.  The loader plugin is dependant on <code>tcl84.dll</code> and will use the first one found in the dll search path.  If you have a cygwin installation (with tcl 8.4), and have added its bin directory to your <code>PATH</code>, then Pidgin may crash on startup.  The solution is to remove cygwin's bin directory from your path.  Introducing cygwin dlls into the native win32 environment is a very bad idea, and is likely to cause problems with other programs.
</li>

<li>
<b>GTK+ 2.8.x Unable to connect</b> - Versions prior to 2.0.0beta4 do not work with GTK+ 2.8.0 or newer.  The symptoms are an inability to connect and/or frequent disconnections.  The issue is specifically in Glib 2.8.x.  For those interested in the technical details, the GIOChannel implementation has been rewritten and appears to have some quirks - see the following <a href="http://bugzilla.gnome.org/show_bug.cgi?id=324234">bugzilla entry</a>.  To avoid this problem, use Pidgin 2.0.0beta4 or newer, or stick with GTK+ 2.6.10.
</li>

<li>
<b>GTK+ 2.6.9 causes GIMP crashes</b> - The updated GTK-Wimp theme that was distributed with our GTK+ 2.6.9 installer (also included in the Pidgin 1.5.0 installer) triggers a bug in GTK+ that causes GIMP to crash.  You should update to <a href="http://prdownloads.sourceforge.net/pidgin/gtk-runtime-2.6.10-rev-a.exe?download">GTK+ 2.6.10</a> to resolve this. See the following <a href="http://bugzilla.gnome.org/show_bug.cgi?id=314529">bugzilla entry</a> for more information.
</li>
<li>
<b>Major GTK+ Bugs</b><i> (that do not need reporting)</i>
<ul>
<li>The text entry cursor is somethimes missing.</li>
<li>Double-clicking the docklet when the buddy list is obscured (i.e. behind another window) will minimize it to the tray rather than bringing it to the front. (<a href="http://bugzilla.gnome.org/show_bug.cgi?id=164311">bugzilla entry</a>)</li>
</ul>
</li>
</ul>


<h2 id="wingtk">Standalone GTK+ Installer</h2>
<a href="http://prdownloads.sourceforge.net/pidgin/gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe?download">gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe</a> (Use <a href="http://prdownloads.sourceforge.net/pidgin/gtk-runtime-2.6.10-rev-a.exe?download">gtk-runtime-2.6.10-rev-a.exe</a> for Win95/98/ME.)

<p>Check <a href="ChangeLog.win32">ChangeLog.win32</a> for the changes in this GTK+ revision.</p>

<h2 id="autoinst">Unattended Installations</h2>
<p>As of 0.73, unattended installations are possible. Simply do the following:</p>
<pre><code>pidgin-<?php print $current_win32_version; ?>.exe /L=1033 /S /D=C:\Pidgin</code></pre>
<p>Make sure to leave the <code>/D</code> flag for last.  The <code>/L</code> flag sets the language (Windows LCID) used by the uninstaller, as well as Pidgin itself.  You may omit both the <code>/L</code> and <code>/D</code> flags and use the defaults which are 1033 (English) and <code>C:\Program Files\Pidgin</code> respectively. The same applies to non-gtk versions. If you need to specify the GTK+ install path (Default is <code>C:\Program Files\Common Files\GTK\2.0</code>), then download the latest GTK+ installer from above, and do the following:</p>
<pre>
<code>
gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe /L=1033 /S /NOUI /D=C:\GTK+
pidgin-<?php print $current_win32_version; ?>-no-gtk.exe /L=1033 /S /D=C:\Pidgin
</code>
</pre>
<p>The GTK+ installer has two silent states. One with message boxes: <code>/S</code>, and one without: <code>/S /NOUI</code>.  The reason for this is that the installer was made to run from within the Pidgin installer where you would expect to see message boxes for errors.</p>

<h2 id="spelling">Spell-checking</h2>
<h3>Aspell and Aspell dictionary installers</h3>
<p>The Pidgin installer will attempt to download and install Aspell and a Aspell dictionary if you choose the appropriate options.  If this isn't successful (e.g. due to a firewall or proxy that prevented the download), you can manually download and install Aspell and a dictionary from here: <a href="http://aspell.net/win32">aspell.net/win32</a>.</p>
<p>Note: Don't forget to turn on "Highlight misspelled words" in the preferences.</p>

<h2 id="perl">Perl Plugin Support</h2>
<p>To enable Perl Plugins, you need to download and install <a href="http://www.activestate.com/Products/Language_Distributions/">ActivePerl</a> 5.8. If your plugins are not working, run <code>pidgin -d</code> and look for errors when loading <code>plugins\perl.dll</code>.  If you see errors about not finding <code>perl58.dll</code>, add the Perl bin dirctory to your <code>PATH</code>.</p>

<h2 id="tcl">TCL Plugin Support</h2>
<p>To enable TCL Plugins, you need to download and install <a href="http://www.activestate.com/Products/Language_Distributions/">ActiveTCL</a> 8.4.5.0.</p>

<h2 id="debug">Debugging Windows Pidgin</h2>
<h3>What to do if Pidgin crashes</h3>
<p> <a href="http://developer.pidgin.im/wiki/TipsForBugReports#WhattodoifPidgincrashes">See wiki</a> </p>

<h2 id="portable">Running Windows Pidgin From a USB Drive</h2>
<p>It is relatively easy to set up Pidgin to run from a USB Drive.  Most of Pidgin is very good about not assuming that it is installed; a slightly customized launcher makes it all come together nicely.</p>
<p>The way that this is set up, other GTK+ applications can use the same GTK+ runtime.</p>
<p>Caveats</p>
<ul>
  <li>Spell-checking requires 2.0.0 beta3 or newer.</li>
</ul>
<h3>How to setup your USB-Drive Pidgin</h3>
<ul>
  <li>Download and Install Pidgin</li>
  <li>Prepare your USB Drive: (This assumes that your USB Drive is <code>U:</code> for the purpose of explanation)
    <ul>
      <li>Create a directory: <code>U:\Programs</code> (This can be whatever you want, the idea is that GTK and Pidgin are at the same level)</li>
      <li>Create <code>GTK</code> and <code>Pidgin</code> directories in this directory.</li>
    </ul>
  </li>
  <li>Copy a minimal part of your Pidgin installation to the USB Drive from the installed instance. The directory structure stays the same, but we don't need everything that is included in the standard installation.
    <ul>
      <li>Copy the whole <code>pixmaps</code> and <code>sounds</code> directories.</li>
      <li>Create a <code>U:\Programs\Pidgin\plugins</code> directory and copy the contents of the installed <code>Pidgin\plugins</code> directory into it excluding <code>perl.dll</code>, <code>tcl.dll</code>.</li>
      <li>If you want to use a language other than en_US, create a <code>U:\Programs\Pidgin\locale</code> directory and copy any language directories that you want from your installed <code>Pidgin\locale</code> directory.</li>
      <li>Copy the contents of your root installed Pidgin directory except <code>pidgin.exe</code>, <code>pidgin-uninst.exe</code>, and if you don't want spell-checking support, <code>libgtkspell.dll</code>.</li>
      <li>Download <a href="pidgin-portable.exe">pidgin-portable.exe</a> (use <a href="gaim-portable.exe">gaim-portable.exe</a> for 2.0.0beta4-6 or <a href="gaim-portable.exe.pre200b4">this version</a> for versions older than 2.0.0beta4) and put it in <code>U:\Programs\Pidgin</code>. The file uses the same source as the standard <code>pidgin.exe</code>, the difference being that when it was compiled <code>PORTABLE</code> was defined which causes some of the initialization to be done differently.</li>
    </ul>
  </li>
  <li>In the same spirit, create a minimal GTK+ runtime installation from your GTK+ installation.
    <ul>
      <li>Copy the <code>bin</code> and <code>etc</code> directories to your <code>U:\Programs\GTK</code> directory.</li>
      <li>Create a <code>U:\Programs\GTK\lib</code> directory and copy the contents of the installed GTK+ runtime's <code>lib</code> directory, except the <code>locale</code> subdirectory.</li>
      <li>If you want to use a language other than en_US, create a <code>U:\Programs\GTK\lib\locale</code> directory and copy any language directories that you want from your installed GTK+ runtime's <code>lib\locale</code> directory.</li>
    </ul>
  </li>
  <li>If you want spell-checking support, you will also need to create a minimal ASpell runtime installation from your ASpell installation.
    <ul>
      <li>Create a <code>U:\Programs\Aspell</code> directory with a <code>bin</code> subdirectory.</li>
      <li>Copy <code>aspell-15.dll</code> into your ASpell <code>bin</code> directory.</li>
      <li>Copy the <code>data</code> and <code>dict</code> subdirectories into <code>U:\Programs\Aspell</code>.</li>
    </ul>
  </li>
  <li>You are done! Running <code>U:\Programs\Pidgin\pidgin-portable.exe</code> will cause pidgin to run off the USB Drive, saving the settings, logs and etc. to <code>U:\Programs\.purple</code>.</li>
</ul>