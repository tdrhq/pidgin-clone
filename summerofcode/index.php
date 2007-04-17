<?
	$page->title = "Summer of Code";
	require('../inc/template.inc.php');
?>


<i>For information about Summer of Code 2006, see <a href='2006.php'>last year's page</a></i>.
<i>For information about Summer of Code 2005, see <a href='2005.php'>that page</a></i>.

<h1>Contents</h1>
<ul>
    <li><a href="#news">News</a></li>
    <li><a href="#intro">Introduction</a></li>
    <li><a href="#accepted">Accepted projects</a></li>
    <li><a href="#available">Available suggestions</a></li>
</ul>

<h1 id="intro">Introduction</h1>
<p>Once again, Google has chosen Pidgin to participate as a mentoring organization in their Summer of Code project. The Summer of Code is like an internship; students apply to work for an open-source project, under the guidance and mentorship of experienced open-souce developers, and upon sucessful completion of their project, the student is paid $4500. Find out more about the Summer of Code at <a href='http://code.google.com/soc'>code.google.com</a>.</p>

<p>Pidgin is current accepting applications from students interested in participating. See below for an in-progress list of our ideas, or come up with an idea of your own.</p>

<h1 id="available">Summer of Code Project Suggestions</h1>

<p>Largely due to the work done by a summer of code student last year, Pidgin is currently separated into three modules, 'gaim' the GTK+ GUI client, 'gaim-text' the console-based client, and 'libgaim' the backend library that powers both of these, and other external projects. This list breaks our ideas between these three. Note that a project that affects libgaim may very well require changes to one or both of the UIs to be useful.</p>

<h2>libgaim</h2>
<table>
<tr><td class="highlight">Improved Privacy Support</td></tr>
<tr><td><p>As Pidgin began life as an AIM client, there are still some parts that are somewhat AIM-centric. Currently, one of the areas most uncomfortable for users of other protocols is privacy support. Your task would be to improve Pidgin's privacy support.</p>
<p>This job entails surveying other IM clients and determining what sort of privacy capabilities they offer, abstracting these all into a concise Pidgin backend API, designing a UI capable of handling the diversity of different privacy features, then finally, implementing and documenting your work.</p>
</td></tr> 

<tr><td class="highlight">Certificate Manager</td></tr>
<tr><td><p>Some of our protocols, namely Jabber, IRC, and MSN, use SSL encryption. However, Pidgin currently has no way to ensure the user that she's actually connecting to the correct server. If a malicious cracker stood between you and your Jabber server, he could trick you into thinking he was the server. You would think your messages are encrypted, but the malicious cracker could read them all.</p>

<p>This is solved by a system known as "certificates." An SSL certificate authenticates that the server is who it claims to be. If the user trusts the certificate, she'll know her communications are safe.</p>

<p>Your task this summer is to write a certificate manager for Pidgin. This manager would display SSL certificates to the user and keep track of those the user has trusted. This way, the first time the user connects to a server, she'll be required to confirm the certificate is valid; subsequent connections to the same server with the same certificate will not prompt her again.</p>

<p>This same interface can be used elsewhere in Pidgin not directly related to SSL. For instance, several third-party plugins exist to encrypt Pidgin messages. These would benefit from a generic certificate manager.</p></td></tr>

<tr><td class="highlight">ICQ TLC</td></tr>
<tr><td><p>Our ICQ implementation in the oscar code is substandard in a variety of respects. We do not have full support for ICQ status, ICQ privacy, or ICQ formatting to name just a few shortfalls. Others include our buddy list management, message size restrictions and ICQ's chats. Your goal is to compare what gaim does at a protocol level with what the official winicq client does, figure out where it differs, and to repair Pidgin's implementation, extending Pidgin's capabilities where possible.</p></td></tr>

<tr><td class='highlight'>MySpace IM protocol plugin</td></tr>
<tr><td><p>Last year MySpace, the popular social-networking site announced its new instant messaging network and protocol. Your goal is to reverse engineer and implement support for this protocol in libgaim. If applying for this project, please demonstrate in your application that you have the knowledge and skills necessary to reverse engineer the protocol used by MySpace's Windows client.</p></td></tr>

<tr><td class='highlight'>GObjectification</td></tr>
<tr><td><p>Pidgin currently uses its own system for connecting to and emitting signals.  For this project you'll convert all major Pidgin components to GObjects.</p></td></tr>

<tr><td class='highlight'>Improved group chat support</td></tr>
<tr><td><p>Some protocols, notably MSN and Yahoo! support un-named chats, or conferences. To date, Pidgin has had fairly poor support for this feature. Pidgin treats this multi-user chats as normal named chats with the name "MSN Chat" for instance. Logs for these chats are not separated by users, and there's no way to easily add a user to a one-on-one IM to turn it into a group chat.</p>

<p>Your goal this summer will be to design and implement an unnamed chat API in libgaim, figure out the details of properly logging them, and support the API in at least one client with support for easily turning a one-on-one chat into a group chat.</p>
</td></tr>
</table>

<h2>gaim</h2>
<table>
<tr><td class="highlight">Better HTML Widget</td></tr>
<tr><td><p>Pidgin currently uses a home-grown text rendering widget called GtkIMHtml.  It has various minor drawbacks, including poor performance with large numbers of tags.  The WYSIWYG text editing code is also a bit delicate and took quite a bit of tuning before it was able to generate HTML that isn't crazy.</p>
<p>For this project you should investigate alternative widgets that are capable of rendering fonts, images, colors, etc. and replace our current widget with something smaller, better, and more elegant.</p>
</td></tr>
</table>

<h2>gaim-text</h2>
<p>Possible improvements to gaim-text could include:
</p>
<table>
<tr><td class='highlight'></td></tr>
<tr><td><p>
<ul>
<li>A new window manager (or improvements to the existing ones). For example, a tiling window manager would be super awesome.</li>
<li>Improve the widget-packing in a container (GntBox), may be even have a grid-like container.</li>
<li>Implement the plugin-pref-ui using the request-api.</li>
<li>Have python (or some other) bindings for libgnt. This would allow creating functional/useful dbus clients with gnt ui.</li>
<li>Add some more items in the buddy-list menu. Copy the gstreamer, auto-reconnection etc. code from gtk-gaim into gaim-text.</li>
<li>Have a spell checker, which would add some way of indicating spelling errors, suggesting possible corrections etc.</li>
<li>Allow adding shortcut keys for the menuitems.</li>
</ul>
</p></td></tr>
</table>
<p> Some of the ideas listed for gaim-text may be too small to be considered for an SoC project. You may want to bundle a few together for a proposal. Also, it'd be really quite nice if you come up with new ideas of your own.</p>
