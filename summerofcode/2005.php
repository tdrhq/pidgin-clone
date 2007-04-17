<?
	$page->title = "Summer of Code";
	require('../inc/template.inc.php');
?>

<h1>Contents</h1>
<ul><li><em><a href="planet">Planet Summer of Gaim</a></em></li>
    <li><a href="#news">News</a></li>
    <li><a href="#intro">Introduction</a></li>
    <li><a href="#accepted">Accepted projects</a></li>
    <li><a href="#available">Available suggestions</a></li>
</ul>

<h1 id="news">News</h1>

<h2 class="news">Summer of Code Ends</h2>
<div class="newsdate">October 12, 2005 - 10:35PM EDT</div>
<p>The "pencils down" date for the Summer of Code was September 1st.  Here's a list of some of the spectacular things the Summer of Code students accomplished during their two months.
</p>
<ul>

<li>
Rendezvous Plugin (Juanjo Molinero Horno) - Wrote a protocol plugin (PRPL) that communicates with Apple's iChat using the Bonjour protocol.  The Bonjour protocol uses multicast DNS to automatically discover other Bonjour users on your local network.  The PRPL currently uses the <a href="http://www.porchdogsoft.com/products/howl/">Howl</a> library for multicast DNS, but we'd like to switch to using <a href="http://www.freedesktop.org/Software/Avahi">Avahi</a> in the future.  This in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
D-Busified gaim-remote (Piotr Zielinski) - Added D-Bus bindings to Gaim which allow other D-Bus aware programs to interact with Gaim.  Removed the old, socket-based gaim-remote executable.  It is being replaced by a python script that communicates with Gaim via D-Bus.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
Improved Perl Scripting (John Kelm) - Fixed the problems with our Perl interpreter and added support for lots of Gaim's newer functionality.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
UPnP NAT Traversal (Adam J. Warrington) - Gaim will now talk to your router and arrange for certain ports to be forwarded to your computer when doing file transfers.  This should <b>greatly</b> improve file transfer success rates.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
Gadu-Gadu Support (Bartosz Oler) - Our Gadu-Gadu protocol plugin is now using a much more recent version of libgadu.  Many many improvements were made, and this protocol plugin is now working very well.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
SIP/SIMPLE (Thomas Butter) - We now have a working protocol plugin for doing IM over a SIP connection, compatable with kphone, iptel.org and sipgate.de.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
ICQ File Transfer (Jonathan Clark) - ICQ file transfer turned out to be pretty easy, since new versions of ICQ use the same file transfer protocol as AIM.  After Jonathan got that working, he went on to add support for proxying a file through AOL's file transfer proxy servers, and made lots of other improvements to the file transfer code used by both AIM and ICQ.  This is in CVS HEAD and will be in the next release of Gaim.  Some of his changes also made it into earlier releases of Gaim.
</li>

<li>
Music Messaging (Christian Muise) - Music Messaging is a Gaim plugin that allows collaborative musical score editing.  It uses the SoC DBus Gaim project to link Gaim with a score editor that is geared to use this functionality.  This requires the DBus plugin to be operational.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
SMS PC-to-mobile routing over Bluetooth (Mel Dooki) - This project does not releate directly to Gaim, but we thought it was neat.  Mel wrote a Java client that runs on a computer.  The user types an SMS, the client transfers the message to a cell phone via Bluetooth, then another program on the cell phone transmits the SMS.  See the <a href="http://routingsms.sourceforge.net/">SMS Routing over Bluetooth</a> webpage for more information.
</li>

<li>
Crazy Chat (Charlie Stockman) - Incomplete, but code IS checked into CVS HEAD.
</li>

<li>
Doodle (Andrew Dieffenbach) - Created a "whiteboard" system for Gaim and used it to implement Yahoo!'s Doodle protocol.  It may need a little more work before it's completely compatable with current Yahoo! clients, but it works fine between two Gaim users.  This is in CVS HEAD and will be in the next release of Gaim.
</li>

<li>
Collaborative Code Editor (Chisthian Kim) - Incomplete.
</li>

</ul>

<h2 class="news">Summer of Code Blogs</h2>
<div class="newsdate">July 7, 2005 - 12:05AM EDT</div>
<p>The Summer of Code students are now journaling their work in blogs. I've linked names to blogs in the project listing below and have aggregated them to a single <a href="planet">page</a>. Thanks to Steven Garrity for the help.</p>

<h2 class="news">Summer of Code Begins</h2>
<div class="newsdate">June 27, 2005 - 6:08PM EDT</div>
<p>The selection process for the Summer of Code has ended. As I mentioned on the <a href="/index.php">front page</a>, it was really hard selecting only 15. I've updated this page to show the selected projects and applicants as well as to show which of our suggestions were not chosen for the summer.</p>
<br />

<h1 id="intro">Introduction</h1>
<p>Gaim is proud to participate in the Google <a href="http://code.google.com/summerofcode.html">Summer of Code</a>. This summer Google is putting up $1 million dollars for students to work on free software projects under the mentorship of experienced free software developers. The Gaim developers are glad to volunteer to mentor some students and introduce them to the world of open-source development.</p>

<p>Students may apply for the program at <a href="http://code.google.com/summerofcode.html">Google's webpage</a> by selecting "Gaim" as their sponsor. Google and Gaim will then work together to select from the candidates. The chosen candidates will then work closely with Gaim developers to complete their project. Upon completion, the student will receive $4500 <b>AND</b> a tee-shirt!</p>

<p>The students may choose any project they like; it doesn't even have to be related to Gaim (although we'd be better able to mentor you if it were). If you have a great idea you've been looking for an opportunity to write, let us know about it! We're especially interested in hearing really innovative, ground-breaking, never-before-heard-of uses of instant messaging. Do something really experimental!</p>

<p>However, if you just want to help out Gaim where the developers think we need help the most (which we'd really appreciate), we've compiled a brief list of projects we'd like to help students complete this summer:</p>

<p>When you apply for a project, the "proposal," section is the only thing we have on which to base our opinion. In that secion alone, you must convince us and Google to fund your proposal. Google links to the Perl foundation's <a href="http://www.perlfoundation.org/gc/grants/proposals.html">HOWTO</a> on writing a proposal. Here's a brief list of things that we especially want to know:</p>
<ul>
<li><b>Academic Merit</b> We're really excited about making this summer an educational experience for all the students involved. Tell us what you expect to learn from your project. We're carefully choosing the projects listed here to ensure you need to actually design stuff from the ground up, despite the temptation to just let Google foot the bill for tedious, mindless tasks we just don't want to do ourselves.</li>
<li><b>Qualifications</b> This is especially true if you apply for one of the project in this list. You will likely be competing against other candidates, so tell us what makes you a better choice than others.</li>
<li><b>Coolness</b> If you have your own idea, just convince us that it's really cool. We're interested in seeing a lot of unique, creative, innovative ideas using IM in ways noone has thought of before. 
</ul>

<p>We will be adding more projects soon, as we think of them!</p>

<h1 id="accepted">Accepted Summer of Code Projects</h1>
<p>The following projects were accepted to receive Google sponsorship in their summer of code project. I've included here a brief synopsis of the project, and the name of the person working on it.</p>

<table>
<tr><td class="highlight">Rendezvous Plugin - <a href="juanjo">Juanjo Molinero Horno</a></td></tr>
<tr><td><p>Apple iChat uses a protocol formerly called Rendezvous (renamed Bonjour due to trademark disputes) to provide a buddy list and messaging among people on the same local network. Gaim developer Mark Doliner has started a Rendezvous plugin, but it is far from release-ready. Your task this summer will be to complete this protocol support.</p>
<p>Rendezvous uses multicast DNS for presence support and the technologies used are all open and documented. Access to a Macintosh running iChat would certainly be useful, though.</p></td></tr>

<tr><td class="highlight">D-Busified gaim-remote - <a href="piotr">Piotr Zielinski</a></td></tr>
<tr><td><p>On UNIX systems, Gaim includes a separate utility called <code>gaim-remote</code>. This utility is used to execute commands on an existing Gaim session. <code>gaim-remote</code> uses a protocol we made up called CUI which was initially designed to allow entire user interfaces communicate with a headless Gaim "core" process. This idea has not been developed at all, but the limitations of that protocol's design prevent <code>gaim-remote</code> from doing things people actually want to do with it. Further, other applications would love to communicate with a Gaim process for desktop integration reasons. Requiring each of these to implement the CUI protocol is unnecessarily cruel to them.</p>
<p>D-Bus is an emerging system for inter-process communication. It provides mechanisms for applications to locate and send messages to each other. Although still in active development, it seems likely that D-Bus will soon be an essiential element of the UNIX desktop.</p>
<p>Your job for the summer will be to rewrite the <code>gaim-remote</code> functionality using D-Bus. As D-Bus becomes <i>the</i> standard for communicating messages between applications, all these applications will have the ability to communicate with Gaim. In addition to duplicating the functionality already present in <code>gaim-remote</code> you will be adding tons more features that can be used to remotely manage a Gaim session (changing status, reading the buddy list, sending messages, etc.).</p></td></tr>

<tr><td class="highlight">Embedded Perl Interpreter - <a href="john">John Kelm</a></td></tr>
<tr><td><p>Gaim has supported perl scripting since September, 2000. Unfortunately, the current embedded perl interpreter is in poor shape due to a rapidly changing API and nobody working on Perl. At present, it is generally not possible to do anything useful in a perl script</p>
<p>Your task this summer will be to bring the perl plugin up-to-date with the current API. This work is mostly done in XS, a system designed for wrapping C functions in perl. Essentially, every function in the Gaim <a href="/api/">plugin API</a> will need to be wrapped in perl. It will probably also help if you develop some way of testing that each function works as it should.</p></td></tr>

<tr><td class="highlight">Embedded Mono Runtime - <a href="andrews">Andrew Scukanec</a></td></tr>
<tr><td><p>Mono is a free software implementation of Microsoft's .NET platform. .NET programs compile to byte code and are then "Just-In-Time" compiled at runtime (this is similar to Java). We would like to see Gaim plugins written using .NET. Because of the byte-code nature of .NET, these plugins could be written in a large number of languages, including C#, and compiled to work on any Gaim, regardless of platform, making plugin distribution much easier.</p>
<p>Your task for the summer is to embed the Mono runtime into Gaim and provide a .NET wrapper to Gaim's API. There exists a lot of good documentation for doing this.</p></td></tr>

<tr><td class="highlight">UPnP NAT Traversal - <a href="adam">Adam J. Warrington</a></td></tr>
<tr><td><p>Tons of people have difficulty transferring files or otherwise directly connecting with Gaim because they're behind a NAT device. NAT devices allow more than one host to share a single IP address and are very useful for users with multiple computers but a single Internet connection. These devices are usually sold as "broadband routers." Unfortunately, because multiple hosts share a single IP address, neither can be addressed specifically without additional (confusing) configuration.</p>
<p>UPnP is a protocol from Microsoft that, among other things, provides NAT traversal: a way to allow each host behind the NAT device to receive incoming connections. Implementing this in Gaim would fix most file transfer problems for people with UPnP-enabled NAT devices (most currently sold are). Your task this summer is to implement just enough of UPnP in Gaim to allow for NAT-transparency.</p></td></tr>

<tr><td class="highlight">Gadu-Gadu Support - <a href="bartosz">Bartosz Oler</a></td></tr>
<tr><td><p>Gadu-Gadu is a Polish instant messaging protocol that Gaim has supported since December, 2001. However, since its original maintainer lost interest, its support in Gaim has lacked the love needed to ensure it works, much less up-to-date with the features of the official Gadu-Gadu client.</p>
<p>Your task this summer will be to bring Gadu-Gadu up-to-date with the functionality of the current official Gadu-Gadu application. Speaking Polish is definitely a plus (actually <i>being</i> Polish and a Gadu-Gadu user even more so). There exist working free software implementations of Gadu-Gadu already, so reverse engineering may not be necessary</p></td></tr>

<tr><td class="highlight">SIP/SIMPLE - <a href="thomas">Thomas Butter</a></td></tr>
<tr><td><p>SIP is another popular protocol used for VoIP, and it also has an IM analog in the form of SIP/SIMPLE. Your task this summer will be to write a plugin that supports SIP/SIMPLE. SIP/SIMPLE is an open protocol with a documented specification.</p></td></tr>

<tr><td class="highlight">ICQ File Transfer - <a href="jonathan">Jonathan Clark</a></td></tr>
<tr><td><p>The ICQ protocol supports file transfer, but Gaim doesn't implement it. Your task this summer is to fully implement ICQ file transfer in Gaim's OSCAR plugin. This may involve reverse engineering the protocol, but there do exist free software implementations you can use as a reference.</p></td></tr>

<tr><td class="highlight">SSL Certificate Validation - <a href="erin">Erin Miller</a></td></tr>
<tr><td>Some of our protocols, namely Jabber, IRC, and MSN, use SSL encryption. However, Gaim currently has no way to ensure the user that she's actually connecting to the correct server. If a malicious cracker stood between you and your Jabber server, he could trick you into thinking he was the server. You would think your messages are encrypted, but the malicious cracker could read them all.</p>
<p>This is solved by a system known as "certificates." An SSL certificate authenticates that the server is who it claims to be. If the user trusts the certificate, she'll know her communications are safe.</p>
<p>Your task this summer is to write a certificate manager for Gaim. This manager would display SSL certificates to the user and keep track of those the user has trusted. This way, the first time the user connects to a server, she'll be required to confirm the certificate is valid; subsequent connections to the same server with the same certificate will not prompt her again.</p>
<p>This same interface can be used elsewhere in Gaim not directly related to SSL. For instance, several third-party plugins exist to encrypt Gaim messages. These would benefit from a generic certificate manager.</p></td></tr>

<!-- Originals -->
<tr><td class="highlight">Music Messaging - <a href="christian">Christian Muise</a></td></tr>
<tr><td><p>Musicians are known to be social, but this rarely presents itself outside of the typical garage jam sessions, all night caleighs at the camp, or swingin blues sets at a smoky bar (you get the idea). With the new age, plenty of software has been made available for musicians, but it is all for independent work, or for those music gatherings.</p>
<p>What I propose is a music meet of the new age. Real-time has been explored (biggest draw back being latency times - but still dealt with formidably), but what about the casual input? When we chat on Gaim it is a social event. So far no developer has tapped into making that social arena appeal to the musician looking to socialize as a musician.</p>
<p>What I envision is software to connect musicians in a suite of different activities they could partake in. From composing music to playing music for others, etc. For the scope of SOC (Summer of Code), I propose a feature added to Gaim that will allow 2 (or more) people to actively edit a score of music in real-time. After which they can export it to MIDI and do as they please with the composition.</p>
<p>The ability to send music, ideas, riffs, and solos is something that a musician always dreams of, and with the technology available today, it's pretty surprising to see the lack of methods available. This is what Music Messaging would aim to overcome.</p></td></tr>

<tr><td class="highlight">A more flexible type of buddylist - <a href="jesper">Jesper Stemann Andersen</a></td></tr>
<tr><td><p>Though groups can help the user sort his buddylist, a buddylist can easily grow to an unmanageable size, without the user wanting to actually delete "buddies". This project proposes a method to identify and group the "buddies" the user actually seems to have an interest in communicating with.</p>
<p>The idea is to use statistics on the users communication patterns to identify relations between the user and "buddies": 1-to-1 IM sessions can relate the user with one single buddy, while
Multi-participant IM sessions, in addition can relate buddies with regard to the user.
This way groups of "buddies" can automatically be identified.</p>
<p>Graphically the groups can be managed by the user. The user is placed in the center of a circle made up of his "buddies". "Buddies" with whom the user communicates a lot, should be positioned closer to the center, while less important "buddies" get pushed to the perifery. If relations has been established between buddies, these buddies can form a cycle of their own, with a center on the circle perifery.</p>
</td></tr>

<tr><td class="highlight">Routing SMS from PC to mobile over Bluetooth to be sent automatically to the desired mobile number - <a href="mel">Mel Dooki</a></td></tr>
<tr><td><p>This project's idea tends to relief people from the tedious task of writing SMS's on mobile phones. 
Instead, the user of this application would be provided with an interface on the computer that allows him/her to write the message on the computer using the keyboard. The message will then be routed from the PC to the mobile phone using Bluetooth link. 
The mobile phone will send the SMS automatically to the desired number. </p></td></tr>

<tr><td class="highlight">Crazy Chat - <a href="charlie">Charlie Stockman</a></td></tr>
<tr><td><p>I plan to update and extend a plugin called Crazy Chat that I made for Gaim for my senior software project at Stanford University. During a Crazy Chat session, the program uses the input from the local user&#x2019;s webcam, and does image processing to detect key facial features such as the eyes and the corners of the mouth.  It uses these features to determine parameters that describe the user&#x2019;s expression, such as head tilt, head distance, mouth shape, and blinking.  These parameters are then sent across the network, and are used to render a 3D &#x201C;cartoony&#x201D; version of the person&#x2019;s face on the remote user&#x2019;s computer (in the current version of Crazy Chat, there is a dog and a shark face). </p>
<p>Crazy Chat has several appealing benefits beyond the inherent fun of being turned into a three dimensional Labrador.  The first is that it is extremely low bandwidth.  For each frame, we only need to send about one hundred bytes, rather than many kilobytes for streaming video.  This could potentially allow for large multi-conference Crazy Chats, in which each user represents a different creature in an online RPG troop.  Another advantage of Crazy Chat over ordinary video conferencing is that it allows the user a higher degree of privacy.  Rolled eyes, bed hair and messy rooms are all suppressed behind the exaggerated smiles of an urban hyena or razor sharp winks of a lady killer whale.  And of course the inherent fun alluded to earlier should not be casually dismissed.  Gaim is first and foremost and entertainment application.</p>
</td></tr>

<tr><td class="highlight">Doodle - <a href="andrewd">Andrew Dieffenbach</a></td></tr>
<tr><td><p>I propose to make a Gaim plugin that allows users to interact with others using the Yahoo Instant Messenger IMVironment service 'Doodle.'</p>
<p>'Doodle' provides the ability for users to draw and sketch together in real-time.  Some subject matter is difficult for many to verbalize; therefore, 'Doodle' is an extremely valuable tool for communication.</p>
<p>Gaim provides users with a unified and unique set of features.  There are only a few which it lacks--a handful of such features one sacrifices while using and supporting Gaim.</p>
<p>The addition of a 'Doodle'-like application for Gaim would widen Gaim's popularity and the open-source platforms it supports as well.</p>
</td></tr>

<tr><td class="highlight">Collaborative Code Editor - <a href="cristhian">Chisthian Kim</a></td></tr>
<tr><td><p>A plug-in for gaim that would allow developers to join a chat room where they can collaboratively edit files specially tailored for coding. It would have color syntaxing for various languages, locking portions of code and it would be highlighted with a color that will be assigned to each user who joins the chatroom, synchronizing of code and much more.</p></td></tr>
</table>

<h1 id="available">Rejected Summer of Code Project Suggestions</h1>

<p>The following projects were suggested by the Gaim team, and, despite numerous excellent applications for each, we decided the ideas were not as strong as some of the original ideas suggested.</p>

<table>
<tr><td class="highlight">RVP Support</td></tr>
<tr><td><p>RVP is the protocol used by Microsoft Exchange messaging. It is used mostly in corporate environments. Your task this summer is to write support for the RVP protocol used by Exchange. The protocol is open and well documented, but to test it, you will need to provide your own access to an Exchange server; no public servers exist.</p></td></tr> 

<tr><td class="highlight">Skype Support</td></tr>
<tr><td><p>The popular Voice Over IP (VoIP) program Skype also supports plain-text instant messaging. Your task for the summer will be to write a plugin that implements this portion of the Skype protocol; you will not need to implement the VoIP protocol. This will probably require reverse engineering the protocol.</p></td></tr>


<tr><td class="highlight">MSN File Transfer</td></tr>
<tr><td>The MSN protocol supports several different file transfer methods which are used under different network conditions. Currently, Gaim only supports a method that transfers through the MSN servers. While this is guaranteed to work under any network condition (even if both sides are behind NAT devices, for instance), it is unbearably slow.</p>
<p>Your task this summer is to implement at least one other MSN file transfer protocol: one that directly connects the two clients. You will also need to provide a mechanism that will easily fallback on the slow through-the-server method in case your new method fails. This may involve reverse engineering, but there are other free software implementations you may use as a reference.</p></td></tr>

<tr><td class="highlight">Improved User Interface</td></tr>
<tr><td>In recent years, the open source world has been focusing increasingly on good user interfaces and effect human-computer interaction.  Some changes have already been made to Gaim to improve its usability, but there is still much work to be done.</p>
<p>Your task this summer is to perform a full review of the Gaim user interface, propose a list of improvements to the gaim-devel mailing list, and then carry out the desired changes by making the appropriate modifications.  Our goal is to make the computer do the work instead of the user.</p>
<p>Knowledge of human computer interaction is a plus.  Some useful references are the <a href="http://developer.gnome.org/projects/gup/hig/">GNOME Human Interface Guidelines</a>, the <a href="http://developer.apple.com/documentation/UserExperience/Conceptual/OSXHIGuidelines/index.html">Apple Human Interface Guidelines</a>, Havoc's essay on <a href="http://www106.pair.com/rhp/free-software-ui.html">Free Software UI</a>, the book <a href="http://www.cooper.com/content/insights/cooper_books.asp#TIARTA"><i>The Inmates are Running the Asylum</i></a>, and any other book on software usability.</p></td></tr>
</table>
