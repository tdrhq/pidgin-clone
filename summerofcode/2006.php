<?
	$page->title = "Summer of Code";
	require('../inc/template.inc.php');
?>

<i>For information about Summer of Code 2005, see <a href='2005.php'>last year's page</a></i>.

<h1>Contents</h1>
<ul>
    <li><a href="#news">News</a></li>
    <li><a href="#intro">Introduction</a></li>
    <li><a href="#accepted">Accepted projects</a></li>
    <li><a href="#available">Available suggestions</a></li>
</ul>

<h1 id="news">News</h1>

<h2 class="news">Halfway Through the Summer</h2>
<div class="newsdate">July 6th, 2006 - 11:58PM PDT</div>
<p>Dig it: Google has awarded us with 8 rockin' summer of code slots, and we've hand-picked 8 rockin' summer of code students.  Keep in mind that these are all still in progress, and while we have high hopes for all of them, it is possible some of these projects will not be completed.  Scroll down a for the full project list, or <a href="#accepted">click here</a>.</p>

<h2 class="news">Summer of Code Begins</h2>
<div class="newsdate">May 1, 2006 - 11:16AM PDT</div>
<p>We are now accepting applications for Google's 2006 <a href='http://code.google.com/soc/'>Summer of Code</a> program. Look at the <a href='#available'>suggestions</a> below, or (even better), come up with your own idea, and consider applying!  To apply, visit Google's <a href="http://code.google.com/soc/student_step1.html">Student Signup</a> webpage.</p>
<br />

<h1 id="intro">Introduction</h1>
<p>Gaim is proud to participate in the Google <a href="http://code.google.com/summerofcode.html">Summer of Code</a>. This summer Google is sponsoring students to work on free software projects under the mentorship of experienced free software developers. The Gaim developers are glad to volunteer to mentor some students and introduce them to the world of open-source development.</p>

<p>Last year Gaim was a very popular mentoring organization, and we had a very difficult time narrowing the candidates down to only fifteen. Many (very many) candidates chose to apply for one of our suggestions, which is fine, but keep in mind that when you apply for one of our suggestions you're competing yourself against hundreds of others for the same idea. When you propose your own idea, you're competing your <i>idea</i> against just a handful of other ideas, as well as highlighting your creativity.</p>

<h1 id="accepted">Accepted Summer of Code Projects</h1>
<table>
<tr><td class="highlight">QQ Support</td></tr>
<tr><td><p>Tim Ringenbach is mentoring Mark Huetsch on this project to bring QQ support to Gaim.  QQ is an extremely popular IM network in China, but doesn't have much of a foothold in the rest of the world.</p></td></tr>

<tr><td class="highlight">MSN Protocol Update</td></tr>
<tr><td><p>Sean Egan is mentoring Ma Yuan on this project to give our MSN code a facelift.  We hope these changes will pave the way for lots of little MSN features that people have been requesting, including status messages.</p></td></tr>

<tr><td class="highlight">Certificate Management</td></tr>
<tr><td><p>Sean Egan is mentoring Gary Sivek for this project.</p></td></tr>

<tr><td class="highlight">Revamped Logging and Reporting</td></tr>
<tr><td><p>Richard Laager is mentoring Bill Reading for this project. Bill is working on modifying the logging functionality to provide for better searching, higher performance, and is also working on implementing a logging plugin that stores logs in a SQL database.</p></td></tr>

<tr><td class="highlight">General Performance Enhancement</td></tr>
<tr><td><p>Ethan Blanton is mentoring Aaron Sheldon for this project.  Aaron is profiling Gaim and making small code changes that fine-tune portions of Gaim's codebase to speed-up some of our more CPU intensive tasks.</p></td></tr>

<tr><td class="highlight">Improved Logging</td></tr>
<tr><td><p>Richard Laager is mentoring Brian Chu for this project. Brian is working on improving logging interoperability with other IM clients. Specifically, this involves collaborating on the design of a unified XML logging format and implementation of an XML logger in Gaim as well as working on a plugin to read the logs from other clients.</p></td></tr>

<tr><td class="highlight">Console-based Gaim using Curses</td></tr>
<tr><td><p>Evan Schoenberg is mentoring Sadrul Habib Chowdhury for this project.  This project consists of taking all the non-GTK parts of Gaim, bundling them into a library, then writing a console-based frontend that utilizes that library to connect to the IM networks in the same way that our current GTK frontend connects.</p></td></tr>

<tr><td class="highlight">Contact Availability Prediction</td></tr>
<tr><td><p>Mark Doliner is mentoring Geoffrey Foster for this project.  Geoffrey is working on a plugin that attempts to predict when your buddies will be online based on their past usage habits.</p></td></tr>
</table>


<h1 id="available">Summer of Code Project Suggestions</h1>

<p>Here are some of the suggestions proposed by Gaim developers for this year's Summer of Code.</p>

<table>
<tr><td class="highlight">Improved Privacy Support</td></tr>
<tr><td><p>As Gaim began life as an AIM client, there are still some parts that are somewhat AIM-centric. Currently, one of the areas most uncomfortable for users of other protocols is privacy support. Your task would be to improve Gaim's privacy support.</p>
<p>This job entails surveying other IM clients and determining what sort of privacy capabilities they offer, abstracting these all into a concise Gaim backend API, designing a UI capable of handling the diversity of different privacy features, then finally, implementing and documenting your work.</p>
</td></tr> 

<tr><td class="highlight">Certificate Manager</td></tr>
<tr><td><p>Some of our protocols, namely Jabber, IRC, and MSN, use SSL encryption. However, Gaim currently has no way to ensure the user that she's actually connecting to the correct server. If a malicious cracker stood between you and your Jabber server, he could trick you into thinking he was the server. You would think your messages are encrypted, but the malicious cracker could read them all.</p>

<p>This is solved by a system known as "certificates." An SSL certificate authenticates that the server is who it claims to be. If the user trusts the certificate, she'll know her communications are safe.</p>

<p>Your task this summer is to write a certificate manager for Gaim. This manager would display SSL certificates to the user and keep track of those the user has trusted. This way, the first time the user connects to a server, she'll be required to confirm the certificate is valid; subsequent connections to the same server with the same certificate will not prompt her again.</p>

<p>This same interface can be used elsewhere in Gaim not directly related to SSL. For instance, several third-party plugins exist to encrypt Gaim messages. These would benefit from a generic certificate manager.</p></td></tr>


<tr><td class="highlight">Profile and Improve Gaim's Performance</td></tr>
<tr><td><p>There are a number of places where Gaim runs slower than it needs to, or takes up more memory than is necessary. Your task this summer is to profile Gaim, determine where it's spending most of its time and memory, and optimize in order to improve performance.</p></td></tr>

<tr><td class="highlight">MSN Protocol Upgrade</td></tr>
<tr><td><p>Newer features of MSN Messenger are available only in protocols greater than what Gaim currently supports. The latest version of the MSN protocol is MSNP13, used by the MSN Messenger 8.0 beta.</p>
<p>Your task this summer is to implement the newest MSN Messenger protocol, and as many of its features as time allows.</p></td></tr>

<tr><td class="highlight">Log Management and Searching</td></tr>
<tr><td><p>Gaim's current logging system is powerful and modularlly extensible, however it would benefit from added functionality. Moving searching into the logger modules, for instance, could greatly improve search performance. The log viewer should also allow you to delete logs directly from the UI. The UI for log management should be improved to reflect your design changes.</p> </td></tr>

</table>
