<?
	$page->title = 'Windows Gaim';
	require('../inc/template.inc.php');
?>

<p><span style="color: #FF0000;">**Note**</span> 1.5.0 does <b>NOT</b> work with MSN and will crash trying to log in.  Use <a href="http://sourceforge.net/project/showfiles.php?group_id=235&package_id=253">2.0.0beta3.1 or newer</a> to avoid this problem.</p>

<h1>Windows Gaim</h1>

<p>Grab the latest <a href="http://prdownloads.sourceforge.net/gaim/gaim-<?php print $current_win32_version; ?>.exe">Windows build (v<?php print $current_win32_version; ?>)</a>, or if you already have GTK+ v<?php print $current_win32_gtk_version; ?> rev <?php print $current_win32_gtk_revision; ?> installed grab <a href="http://prdownloads.sourceforge.net/gaim/gaim-<?php print $current_win32_version; ?>-no-gtk.exe">Windows Gaim without GTK+</a>.</p>

<p>For the latest Windows Gaim specific changes, see <a href="ChangeLog.win32">ChangeLog.win32</a>. It includes information on GTK+ changes/fixes affecting Windows Gaim.</p>

<p>Windows Gaim users/developers are encouraged to congregate in #wingaim on irc.freenode.net.</p>

<p>
<b>NOTE:</b> Those who currently use <code>HOME</code> and <code>LANG</code> environmental variables to set Gaim's settings path and language, will need to use <code>GAIMHOME</code> and <code>GAIMLANG</code> as of version 1.1.0.
</p>

<h3>Contents</h3>
<ul>
<li><a href="#faq">Windows Gaim FAQ</a></li>
<li><a href="#bugs">Major Windows Gaim Bugs &amp; Workarounds</a></li>
<li><a href="#wingtk">Standalone GTK+ Installer</a></li>
<li><a href="#autoinst">Unattended Installations</a></li>
<li><a href="#spelling">Windows Gaim Spell-checking</a></li>
<li><a href="#perl">Perl Plugin Support</a></li>
<li><a href="#tcl">TCL Plugin Support</a></li>
<li><a href="#debug">Windows Gaim Crash Reporting - Howto</a></li>
<li><a href="http://developer.pidgin.im/wiki/BuildingWinPidgin">Windows Build Instructions</a></li>
<li><a href="#portable">Running Gaim from a USB Drive</a></li>
</ul>

<h2 id="faq">Windows Gaim FAQ</h2>
<ul>
<li>
<b>How does Win Gaim determine which language to use?</b> - For the user who installed Win Gaim, the language selected during installation will be the default.  This can be overridden by setting the <code>GAIMLANG</code> (as of v1.1.0) environmental variable (for example <code>GAIMLANG=de</code>).  If both of these methods fail, Win Gaim will fall back to the default Windows user locale setting.
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
<b>Win Gaim Smiley Themes</b> - You can drag and drop the <a href="http://sourceforge.net/tracker/?atid=746976&amp;group_id=235&amp;func=browse">Smiley Themes</a> into Win Gaim.  Simply download a theme to your machine (i.e. to your Desktop), bring up the Smiley theme list in Gaim's Preferences, and then drag and drop the saved theme into the Smiley themes list.  You do not need to keep the file you downloaded.
</li>

<li>
<b>How do I configure Norton Internet Security for Gaim?</b> - Unfortunately, Norton Internet Security doesn't correctly respect specifying "Permit All" for a program, and the following additional settings are necessary for Gaim to function properly.  Symantec has be notified of the problem, but attempts to get it resolved have been fruitless.
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
<b>How do I run multiple instances of Gaim?</b> - As of 1.1.0, Gaim will prevent multiple instances from running simultaneously.  You can override this restriction by defining the <code>GAIM_MULTI_INST</code> environmental variable.  If you do this, you may find the <code>-c</code> parameter useful to specify unique configuration directories for each instance (e.g. <code>-c c:\home\.gaim</code>).
</li>

<li>
<b>How do I upgrade Gaim?</b> - Close any existing Gaim instances.  Install the new version of Gaim.  Start Gaim.  The installer should detect your existing Gaim instllation and default to that directory, so you should not need to change the installation directory.
</li>

<li>
<b>Why do I get a &quot;No disk in drive&quot; (or similar) message?</b> - **This has been fixed in the GTK+ 2.10.6 installer.** There is a bug in one of the components of the GTK+ runtime (gettext).  As a result, this component attempts to locate a configuration file on the I: drive.  If you have an I: drive and it is a removable media drive such as a ZIP drive, CD/DVD-ROM drive, or flash card reader, you will see the an error message stating that there is no disk in the drive or asking you to insert a disk into the drive.  If you can change your drive letters so that you have no I: (possible on Windows 2000 or XP), unplug the device if it is a USB device, or insert a disk or card into the drive before starting Gaim, this should silence the message.  We hope to have a fixed GTK+ runtime soon.  This error message can safely be ignored without any ill effects.
</li>

</ul>

<h2 id="bugs">Major Windows Gaim Bugs and Workarounds</h2>
<ul>
<li>
<p><b>DLL Hell and Win Gaim</b> - A very common but very unpleasant Windows SNAFU (known as DLL Hell) has been experienced by many Win Gaim users.</p>

<p><b>Symptom:</b> Win Gaim crashes routinely on startup (but not neccessarily just on startup).</p>

<p><b>Cause:</b> Dlls with the same name as those that Gaim uses are found in the dll search path and are used instead of those installed by Gaim, likely causing Gaim to malfunction. This can happen when application developers install their dlls to Windows system directories.  The most commonly used dll to cause dll hell is <code>zlib.dll</code>.</p>

<p><b>Solution</b>: As of 0.72, depending on the windows version Gaim will either change working directory before loading <code>gaim.dll</code>, or will use <code>SetDllDirectory</code>.</p>

<p>Why did we move to using the GTK installer? More and more GTK apps are being ported to Windows, and rather than having each app install its own copies of the GTK dlls, it is better that GTK is installed in a common location to avoid having many duplicates of the same dll.  There is more in-depth info about DLL Hell in <a href="http://www.desaware.com/Articles/DllHellL3.htm">this article</a>.</p>

<p>Bug report <a href="http://sourceforge.net/tracker/index.php?func=detail&amp;aid=715975&amp;group_id=235&amp;atid=100235">#715975</a> is one instance of the problem just described. There are more..</p>
</li>
<li>
<b>TCL Loader Plugin + Cygwin = WinGaim crash</b> - Starting with 0.75, Win Gaim comes with a tcl plugin loader.  The loader plugin is dependant on <code>tcl84.dll</code> and will use the first one found in the dll search path.  If you have a cygwin installation (with tcl 8.4), and have added its bin directory to your <code>PATH</code>, then WinGaim will crash on startup.  The solution is to remove cygwin's bin directory from your path.  Introducing cygwin dlls into the native win32 environment is a very bad idea, and is likely to cause problems with other programs.
</li>

<li>
<b>GTK+ 2.8.x Unable to connect</b> - Versions of WinGaim prior to 2.0.0beta4 do not work with GTK+ 2.8.0 or newer.  The symptoms are an inability to connect and/or frequent disconnections.  The issue is specifically in Glib 2.8.x.  For those interested in the technical details, the GIOChannel implementation has been rewritten and appears to have some quirks - see the following <a href="http://bugzilla.gnome.org/show_bug.cgi?id=324234">bugzilla entry</a>.  To avoid this problem, use WinGaim 2.0.0beta4 or newer, or stick with GTK+ 2.6.10.
</li>

<li>
<b>GTK+ 2.6.9 causes GIMP crashes</b> - The updated GTK-Wimp theme that was distributed with the Gaim GTK+ 2.6.9 installer (also included in the WinGaim 1.5.0 installer) triggers a bug in GTK+ that causes GIMP to crash.  You should update to <a href="http://prdownloads.sourceforge.net/gaim/gtk-runtime-2.6.10-rev-a.exe?download">GTK+ 2.6.10</a> to resolve this. See the following <a href="http://bugzilla.gnome.org/show_bug.cgi?id=314529">bugzilla entry</a> for more information.
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
<a href="http://prdownloads.sourceforge.net/gaim/gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe?download">gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe</a>

<p>Check <a href="ChangeLog.win32">ChangeLog.win32</a> for the changes in this GTK+ revision.</p>

<h2 id="autoinst">Unattended Installations</h2>
<p>As of 0.73, unattended installations are possible. Simply do the following:</p>
<pre><code>gaim-<?php print $current_win32_version; ?>.exe /L=1033 /S /D=C:\Gaim</code></pre>
<p>Make sure to leave the <code>/D</code> flag for last.  The <code>/L</code> flag sets the language (Windows LCID) used by the uninstaller, as well as Gaim itself.  You may omit both the <code>/L</code> and <code>/D</code> flags and use the defaults which are 1033 (English) and <code>C:\Program Files\Gaim</code> respectively. The same applies to non-gtk versions. If you need to specify the GTK+ install path (Default is <code>C:\Program Files\Common Files\GTK\2.0</code>), then download the latest GTK+ installer from above, and do the following:</p>
<pre><code>gtk-runtime-<?php print $current_win32_gtk_version; ?>-rev-<?php print $current_win32_gtk_revision; ?>.exe /L=1033 /S /NOUI /D=C:\GTK+
gaim-<?php print $current_win32_version; ?>-no-gtk.exe /L=1033 /S /D=C:\Gaim</code></pre>
<p>The GTK+ installer has two silent states. One with message boxes: <code>/S</code>, and one without: <code>/S /NOUI</code>.  The reason for this is that the installer was made to run from within the Gaim installer where you would expect to see message boxes for errors.</p>

<h2 id="spelling">Gaim Spell-checking</h2>
<h3>Aspell and Aspell dictionary installers</h3>
<p>I will no longer support the aspell and aspell dict installers that I had previously posted here. From now on, Gaim will use the win32 aspell distribution from <a href="http://aspell.net/win32">aspell.net/win32</a>.  To make use of Win Gaim's spell-checking feature you will need to download and install the aspell installer and at least one language dictionary.  If you choose not to install Aspell, Gaim will still work.  Gaim will also work with Aspell from previous Gaim installations.</p>
<p>Note: Don't forget to turn on "Highlight misspelled words" in the preferences.</p>

<h2 id="perl">Perl Plugin Support</h2>
<p>To enable Perl Plugins in Win Gaim, you need to download and install <a href="http://www.activestate.com/Products/Language_Distributions/">ActivePerl</a> 5.8, before installing Gaim (v0.69 and higher). If Gaim complains about not finding <code>perl58.dll</code>, add the Perl bin dirctory to your <code>PATH</code>.</p>

<h2 id="tcl">TCL Plugin Support</h2>
<p>To enable TCL Plugins in Win Gaim, you need to download and install <a href="http://www.activestate.com/Products/Language_Distributions/">ActiveTCL</a> 8.4.5.0. Unlike Perl, you may install TCL after Gaim has been installed.</p>

<h2 id="debug">Debugging Windows Gaim</h2>
<h3>What to do if Gaim crashes</h3>
<p>First make sure that you are not affected by 'Dll Hell' as described above. Download and install the debug version of WinGaim (<a href="http://prdownloads.sourceforge.net/gaim/gaim-<?php print $current_win32_version; ?>-debug.exe"><?php print $current_win32_version; ?> is here</a>; for other versions, go to <a href="http://sourceforge.net/project/showfiles.php?group_id=235&package_id=253">the gaim file releases area on SF.net</a>).  The next time you run Gaim and it crashes, the call stack will be dumped to <code>gaim.RPT</code> in your Gaim install dir.  When submitting a bug report, give a detailed description on how to produce your bug and attach the most recent crash dump from your <code>gaim.RPT</code> file.</p>
<h2 id="portable">Running Windows Gaim From a USB Drive</h2>
<p>It is relatively easy to set up Gaim to run from a USB Drive.  Most of Gaim is very good about not assuming that it is installed; a slightly customized launcher makes it all come together nicely.</p>
<p>The way that this is set up, other GTK+ applications can use the same GTK+ runtime.</p>
<p>Caveats</p>
<ul>
  <li>Spell-checking requires 2.0.0 beta3 or newer.</li>
</ul>
<h3>How to setup your USB-Drive Gaim</h3>
<ul>
  <li>Download and Install Gaim</li>
  <li>Prepare your USB Drive: (This assumes that your USB Drive is <code>U:</code> for the purpose of explanation)
    <ul>
      <li>Create a directory: <code>U:\Programs</code> (This can be whatever you want, the idea is that GTK and Gaim are at the same level)</li>
      <li>Create <code>GTK</code> and <code>Gaim</code> directories in this directory.</li>
    </ul>
  </li>
  <li>Copy a minimal part of your Gaim installation to the USB Drive from the installed instance of Gaim. The directory structure stays the same, but we don't need everything that is included in the standard installation.
    <ul>
      <li>Copy the whole <code>pixmaps</code> and <code>sounds</code> directories.</li>
      <li>Create a <code>U:\Programs\Gaim\plugins</code> directory and copy the contents of the installed <code>Gaim\plugins</code> directory into it excluding <code>perl.dll</code>, <code>tcl.dll</code>.</li>
      <li>If you want to use a language other than en_US, create a <code>U:\Programs\Gaim\locale</code> directory and copy any language directories that you want from your installed <code>Gaim\locale</code> directory.</li>
      <li>Copy the contents of your root installed Gaim directory except <code>gaim.exe</code>, <code>gaim-uninst.exe</code>, and if you don't want spell-checking support, <code>libgtkspell.dll</code>.</li>
      <li>Download <a href="gaim-portable.exe">gaim-portable.exe</a> (use <a href="gaim-portable.exe.pre200b4">this version</a> for versions of gaim older than 2.0.0beta4) and put it in <code>U:\Programs\Gaim</code>. The file uses the same source as the standard <code>gaim.exe</code> from versions 2.0.0 and newer, the difference being that when it was compiled <code>PORTABLE</code> was defined which causes some of the initialization to be done differently.</li>
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
  <li>You are done! Running <code>U:\Programs\Gaim\gaim-portable.exe</code> will cause gaim to run off the USB Drive, saving the settings, logs and etc. to <code>U:\Programs\.gaim</code>.</li>
</ul>
