<?
	$page->title = 'Translation HowTo';
	require('inc/template.inc.php');
?>

<h1>Translation HowTo</h1>

<p>We are currently very informal and very hands-off when it comes to translating Pidgin.  What follows is a collection of notes for those who wish to assist.</p>

<ul>
 <li>We try to accept tanslations for a given language only from a single person, the person we are most familiar with getting translations from.  If you feel a translation is wrong, produces strings that are too long, or is badly out of date, contact the existing translator first.  Work with him or her if possible.  If this is not possible, mention this to us as you submit your patch or new translation.</li>
 <li>Translations to new languages are always welcome.
  <ol>
   <li>Get the po template (.pot) file from <a href="http://pidgin.im/l10n/pidgin.pot" title=".pot file for Pidgin">here</a>.  Alternately run <code>intltool-update --pot</code> in the <code>po/</code> directory.</li>
   <li>Find the two letter language code for your language.  If translating to a regional dialect, append the two letter region or country code.  Rename the file to match this.  For example, Protuguese is pt.po, and Brazilian Portuguese is pt_BR.po.</li>
   <li>Subscribe to our <href="http://pidgin.im/cgi-bin/mailman/listinfo/translators" title="Translators Mailing List">translators mailing list</a></li>
   <li>Open the translation file you downloaded in a text editor</li>
   <li>translate the strings
   <ul>
   <li>Translated .po files may be submitted in whatever native encoding is
   most convenient (and hopefully canonical).  Please ensure that the
   Content-type: line reflects the proper character set.</li>
   <li>Please check your translations carefully.  Make sure you have the same
   number of newlines and %s or other formatting codes in the translation as
   you have in the original.  The po/check_po.pl script can help with this.</li>
   <li>Before submitting updated or new translations, please run the following
   command to ensure the file does not contain errors: <code>msgfmt -c --statistics xx.po</code></li>
   <li>The comments at the top of each .po file should appear as follows:<br/>
   <pre>
   # Pidgin Xxxxx translation
   # Copyright (C) 2002, Another Name &lt;email@something.com&gt;
   # Copyright (C) 2003, Your Name &lt;email@whatever.com&gt;
   #
   # This file is distributed under the same license as the Pidgin package.
   #
   </pre></li>
   </ul></li>
   <li>Post partial or completed files to <a href="http://developer.pidgin.im/newticket" title="trac">trac</a></li>
   <li>Along with your first submission, note what name(s) and email address(es) should be put in help-&gt;about.</li>
  </ol>
 <li>String freezes are announced on the Pidgin translators mailing list</li>
</ul>

<h1>Third Party Plugin Translation HowTo</h1>
<p>So, Pidgin is 100% translated in your language and there are still
untranslated strings.  These strings are most likely owned by a third party
plugin you have installed.  Following the below steps should help you to get
those strings translated.  However, each third party plugin may be run
differently, so some of these suggestions may not apply.
</p>

<ul>
	<li>Determine if the plugin has translation support.  The easiest way to
		determine this is to check the plugin's website if it has one.  If it
		doesn't, try contacting the developers.  If they are unsure of how to
		implement translations support, point them
		<a href="/api/plugin-i18n.html">here</a>.
	</li>
	<li>Determine if a translation already exists.  If it does contact the
		original author and see if they could use your help.  In the event
		that they are no longer maintaining it, contact the developers of the
		plugin, and let them know that you would like to take over maintaining
		the translation.
	</li>
	<li>If a translation does not already exist, follow the below steps:
		<ol>
			<li>Acquire the po template (.pot) for the plugin (note that this
				file and its location will vary for each plugin).
			</li>
			<li>Find the two letter language code for your language. If
				translating to a regional dialect, append the two letter region
				or country code. Rename the file to match this. For example, 
				Protuguese is pt.po, and Brazilian Portuguese is pt_BR.po.
			</li>
			<li>If the plugin has a translation mailing list, or possibly even
				a developers list, subscribe to it.
			</li>
			<li>Open the translation file in a text editor.</li>
			<li>Translate the strings.
				<ul>
					<li>Translated .po files may be submitted in whatever native
						encoding is most convenient (and hopefully canonical).
						Please ensure that the Content-type: line reflects the
						proper character set.
					</li>
					<li>Please check your translations carefully.  Make sure
						you have the same number of newlines and %s or other
						formatting codes in the translation as you have in the
						original.  The po/check_po.pl script can help with this.
					</li>
					<li>Before submitting updated or new translations, please
						run the following command to ensure the file does not
						contain errors: <code>msgfmt -c --statistics xx.po</code>
					</li>
					<li>The comments at the top of each .po file should appear as
						follows:<br/>
<pre># PLUGIN_NAME Xxxxx translation
# Copyright (C) 2002, Another Name &lt;email@something.com&gt;
# Copyright (C) 2003, Your Name &lt;email@whatever.com&gt;
#
# This file is distributed under the same license as the PLUGIN_NAME package.
#</pre>
					</li>
				</ul>
			</li>
			<li>Submit the translation to the developers.  This again will vary
				from plugin to plugin.
			</li>
		</ol>
	</li>
	<li>Listen for string freezes.  Again, this will vary from plugin to plugin,
		and may even been non-existant.
	</li>
</ul>
