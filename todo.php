<?php 
	$page->title = 'Todo';
	require('inc/template.inc.php');
  ?><h1>Pidgin Todo</h1><p align="center"><b>Priorities:</b><font color="#FF0000">Very High</font>,
   <font color="#660000">High</font>,
   <font color="#000000">Medium</font>,
   <font color="#000066">Low</font>,
   <font color="#0000FF">Very Low</font></p><h2>UI stuff</h2><ul>
<li><font color="#FF0000">kicking someone in #name-extension causes a kick to display in #name as well and vice-versa</font></li>
<li>
<font color="#FF0000">SEARCH FUNCTION FOR LOG FILES:</font><ul><li><font color="#FF0000">current function only highlights every other entry (sf bug 980075)</font></li></ul>
</li>
<li><font color="#660000">fix crash when someone writes just as you close the chat window.</font></li>
<li><font color="#660000">Save buddy icon option does not work, it saves a screenname.icon file which is not openable.</font></li>
<li><font color="#000000">status report w/out being on own list</font></li>
<li><font color="#000000">option not to raise windows on event while away</font></li>
<li><font color="#000000">make entry widget height preference a relative size, that is it doesn't change the overall height of the window.</font></li>
<li>
<font color="#000000">buddy pounce stuff:</font><ul><li><font color="#000000">drop down list of buddies</font></li></ul>
</li>
<li><font color="#000000">blist does not resize correctly on some platforms. this does not seem to be an issue on linux.</font></li>
<li><font color="#000000">better gtk2-ified icons: maybe a send icon? review the ones we're using now. do they make sense?</font></li>
<li>
<font color="#000000">do not automatically resize windows that have been manually resized.:</font><ul>
<li><font color="#000000">when a buddy icon is received</font></li>
<li><font color="#000000">when an im image is received</font></li>
<li><font color="#000000">when a new tab is created</font></li>
</ul>
</li>
<li><font color="#000066">block buddy option from right click menu in blist</font></li>
<li><font color="#000066">blist short cuts apparently not working</font></li>
<li><font color="#000066">display status overlay for buddies on the blist that have been blocked</font></li>
<li><font color="#0000F0">option to have input box scroll instead of line wrap. input box only.</font></li>
<li><font color="#0000F0">Transparency for GtkIMHtml</font></li>
</ul><h2>Prefs stuff</h2><ul>
<li><font color="#000000">preference to set logfile path. (i think there is a patch to add this)</font></li>
<li><font color="#000066">add ircproxy option to proxy preferences.</font></li>
<li><font color="#000066">per-sn controls on how to compute idle time</font></li>
</ul><h2>Core stuff</h2><ul>
<li>
<font color="#FF0000">PERMIT ONLY PEOPLE ON MY LIST TO IM ME IS NEEDED ASAP!!!!!!!!:</font><ul>
<li><font color="#FF0000">for yahoo</font></li>
<li><font color="#FF0000">icq</font></li>
<li><font color="#000000">msn</font></li>
</ul>
</li>
<li><font color="#000000">documentation needed. (both online and not)</font></li>
<li><font color="#000000">remember previous state better. both away, and allow start to invisible.</font></li>
<li><font color="#000000">better privacy controls.</font></li>
<li><font color="#000000">networking code rethink (libgnet?)</font></li>
<li><font color="#000066">Cancelable proxy_connect</font></li>
<li><font color="#000066">encryption and hash stuff currently in Pidgin consolidated to an api</font></li>
</ul><h2>away rethink</h2><ul><li><font color="#000000">Away needs to be modified to include "show" and "status", i.e. you can have an away state, and an away message. This is exactly what ICQ does - you're "N/A" (show in Jabber) and your away message is "I'm gone." (status in Jabber). Since it only applies to ICQ and Jabber (and actually, currently only Jabber, since icqlib doesn't handle away messages yet), there should be some way of indicating that it can do both without breaking the others. My suggestion is to put "/" at the end of the "state", and when that's there, have it be a menu with all of the custom messages, e.g. in Jabber it'd return "Away/" as one of the states, and you'd see "Away" with a menu of all the away messages you have.</font></li></ul><h2>WinPidgin specific stuff</h2><ul>
<li><font color="#660000">winPidgin: Gtk windows that appear when you hover over a button, and just stay there and never close until you click on them once</font></li>
<li><font color="#660000">with 3 or more tabs, only the right most tab is closable. this appears to be a gtk for win32 problem</font></li>
<li><font color="#000000">winPidgin font issues</font></li>
<li><font color="#000000">winPidgin: sounds while away?</font></li>
<li><font color="#000066">Write a seperate program that implements multiple-users. It should encrypt/decrypt .Pidginrc's based on a provided password, then launch Pidgin -f, with that .Pidginrc.</font></li>
</ul><h2>Pidgin-remote stuff</h2><ul>
<li><font color="#000066">purple-REMOTE: need a doc written up for Pidgin-remote on usage</font></li>
<li><font color="#000066">purple-REMOTE: need to implement everything listed in Pidgin-remote --help</font></li>
</ul><h2>In General (i don't know where this should be)</h2><ul>
<li><font color="#660000">fix memory leaks</font></li>
<li><font color="#000000">gzip storage of logs</font></li>
<li><font color="#000000">throwaway away message (unsaved) for single account (vs. set all away)</font></li>
<li><font color="#000000">add state saving and hook it to session management and user dialogs</font></li>
<li>
<font color="#000066">importing/exporting blist files:</font><ul>
<li><font color="#000000">export one account's worth of blist.xml</font></li>
<li><font color="#000000">import whatever we export, to any account</font></li>
<li><font color="#000066">licq</font></li>
<li><font color="#0000F0">trillian</font></li>
</ul>
</li>
<li><font color="#000066">adding and removing buddies needs cleaned up. basically, add_buddy should be calling ui_add_buddy and serv_add_buddy as needed (require an extra arg to add_buddy to tell it what needs done) and the various ways a buddy can come to be needing added should be calling some type of request_add_buddy (this would be part of both the core&lt;-&gt;ui interface and the core&lt;-&gt;prpl interface)</font></li>
<li><font color="#000066">some way to ask about differences bettween server lists and local lists</font></li>
</ul><h2>Plugins/Scripts</h2><ul>
<li>
<font color="#000000">sean's plugin ideas:</font><ul>
<li><font color="#000000">screenshot plugin</font></li>
<li><font color="#000000">xmms remote plugin</font></li>
<li><font color="#000000">statusimage plugin</font></li>
</ul>
</li>
<li>
<font color="#000066">docklet stuff:</font><ul><li><font color="#000000">make the docklet support blinking status icons where appropriate (requires setting up a better purple_STOCK_ICON framework)</font></li></ul>
</li>
</ul><h2>i18n/portability stuff</h2><ul></ul><h2>DISCUSSION: entries here are things i'm not sure are desirable or doable.</h2><ul>
<li><font color="#000066">"console beep" sound option should use different frequency beeps for different events, or at least give some way to allow this behavior.</font></li>
<li><font color="#000066">ximian serial numbers in rpms cause... problems.</font></li>
<li><font color="#000066">msn auth expires (better, but still not perfect)</font></li>
<li><font color="#000066">Pidgin should detect if message is sent and only log on success for protocols capable of offline messaging, this would always log, but aim messages would only be logged if sent to an online person. this would essentially entail making logging dependent on the return status of the serv send msg command.</font></li>
<li><font color="#000066">the privacy dialog has five different options now. this looks rather ugly. When we redesign the dialog, perhaps we should change the privacy setting to a combobox?</font></li>
<li><font color="#0000F0">protocol action to log into webmail for those that support such (aim, yahoo, msn) maybe a right-click option to send to user(yahoo, msn)</font></li>
<li><font color="#0000F0">show alias in list of people in chat</font></li>
<li><font color="#0000F0">tab complete to alias, translate to nick on send for chat</font></li>
<li><font color="#0000F0">sorting of conversation tabs, if the ordering of left-to-right order by opening isn't good enough for you</font></li>
<li><font color="#0000F0">option to disable close for x seconds after receive im. this would short circuit missing messages as you close the window. make it an option that's off by default.</font></li>
<li>
<font color="#0000F0">options on key bindings. (different window managers map different things).:</font><ul>
<li><font color="#0000F0">make the control key switchable with the alt key or similar)</font></li>
<li><font color="#0000F0">maybe make control/alt+arrow optionally replace control+bracket (since control-pageup and control-pagedown work, i'm inclined not to do this)</font></li>
</ul>
</li>
<li><font color="#0000F0">make the option to have escape close a window be global (error messages, account editor, etc.)</font></li>
</ul><h2>autotools/gettext</h2><ul>
<li><font color="#0000F0">automake 1.4 support bug, non-root users can't get Pidgin to compile by putting the gtk2 and glib2 m4 files in the m4 dir</font></li>
<li><font color="#0000F0">automake 1.[56] support not perfect.</font></li>
</ul>
