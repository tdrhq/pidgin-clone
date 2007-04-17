<?
	$page->title = 'Protocols';
	require('inc/template.inc.php');
?>

<h1>Protocols</h1>

<ul>
 <li><a href="#icq">ICQ</a></li>
 <li><a href="#irc">IRC</a></li>
 <li><a href="#jabber">Jabber</a></li>
 <li><a href="#msn">MSN</a></li>
 <li><a href="#Novell">Novell GroupWise</a></li>
 <li><a href="#oscar">Oscar ("AIM/ICQ")</a></li>
 <li><a href="#Sametime">Lotus Sametime</a></li>
 <li><a href="#toc">TOC</a></li>
 <li><a href="#yahoo">Yahoo!</a></li>
 <li><a href="#zephyr">Zephyr</a></li>
</ul>

<a name="icq"></a>
<h2>ICQ</h2>
<p>
 The ICQ protocol was originally developed by Mirabilis, Inc., which was
 later bought by AOL. Although the protocol was never open, there have been
 several groups which have attempted to reverse-engineer it. Most of them
 are cooperating now, sharing the information they've obtained about it.
 Purple is able to use ICQ thanks to icqlib, which has taken most of the
 information gathered and put it into a library.
</p>
<p>
 ICQ is based on both UDP and TCP. The connection with the server is UDP,
 and messages such as who's online are sent via UDP. When you have a
 conversation with someone, you connect to them using TCP, and messages are
 sent directly. It is possible to send messages through the server in cases
 where direct TCP connections aren't possible. Direct TCP connections are
 also established for file transfers and chats.
</p>
<p>
 ICQ99 was the last official ICQ client to use the UDP-based protocol.
 Starting with ICQ2000, the official ICQ client uses a modified version of
 <a href="#oscar">Oscar</a>. It is now possible (and recommended) to use
 <a href="#oscar">Oscar</a> to connect to the ICQ network instead of
 ICQ/libicq.  In fact, we recommend this so strongly that we've stopped
 officially distributing the protocol plugin for ICQ.
</p>

<a name="irc"></a>
<h2>IRC</h2>
<p>
 IRC is one of the oldest communication systems; RFC 1459, which documents
 the protocol, makes reference to its initial design work starting in 1989.
 Depsite being standardized in an RFC, several IRC servers deviate slightly
 from the specification, though generally only enough to irk IRC client
 developers.
</p>
<p>
 IRC is a TCP-based protocol, and is completely text-based. It's possible
 to use a standard telnet client as an IRC client if you know the protocol
 well enough. IRC historically has been used mainly for chat rooms, while
 AIM and ICQ have been primarily for one-on-one messaging; however, while
 AIM and ICQ both provide group chats, IRC also provides private messages.
 IRC was not designed with a "buddy list" concept in mind, though; people
 are able to change their username while they are connected to the server,
 which makes having a buddy list difficult. However, because IRC is used
 mainly for chat rooms, if you want to know if someone is available, it's
 usually as easy as just joining a chat room they frequent. Often users
 will also modify their username to indicate state - warmenhoven might
 change his nick (username) to warmenhoven_Asleep to indicate that he's
 connected but not present.
</p>

<a name="jabber"></a>
<h2>Jabber</h2>
<p>
 Jabber is the coolest IM system on the planet (it says so on the
 Jabber.org website). Originally designed by Jeremie Miller, Jabber has
 grown to be the largest and most successful Open Source IM System
 available. The Jabber servers and source code are available for download,
 as are clients for nearly every OS. Purple can act as a Jabber client.
</p>
<p>
 Jabber usernames are reminiscent of email addresses: a typical Jabber
 username would be warmenhoven@jabber.org or robflynn@jabber.com. Jabber
 also lets you connect using the same username multiple times, using what
 they term a "resource". A resource is an arbitrary string, often seen
 appended to your username, e.g. warmenhoven@jabber.org/Purple (Purple in this
 case being the resource), which lets the server keep track of how many
 times that username is signed on.
</p>
<p>
 The Jabber protocol is TCP-based and XML-encoded. Because it is based in
 XML, it is the most extensible IM system available. Users form one TCP
 connection to the server in the duration of the session. Passwords can be
 encoded in a variety of different ways; however, currently Purple only
 allows plain text (i.e. no encryption of passwords). All communication is
 routed through the Jabber servers. It is possible to talk to a user that
 is connected to a different server than you are, assuming both servers
 allow it.
</p>
<p>
 Jabber also has what they call a "transport". A transport is a way of
 connecting to another IM service, such as AIM or ICQ. In order to use a
 transport, you "subscribe" to it, which is sort of like adding it to your
 buddy list, except you must also provide a valid username and password
 (e.g. if you were subscribing to the AIM transport, you would tell it your
 AIM username and the password for that account). Once you have subscribed
 to a transport, it will sign onto that service using the specified
 username, and you can talk to people using that service - their username
 would be ewarmenhoven@aim.jabber.org, for example. The username they would
 see is the one you subscribed to the transport with. Currently in Purple,
 you cannot subscribe to a transport - however, if you subscribe to a
 transport using a different client, Purple can make use of it.
</p>

<a name="msn"></a>
<h2>MSN</h2>
<p>The MSN protocol was originally published by Microsoft as an
<a href="http://www.hypothetic.org/docs/msn/ietf_draft.txt">IETF draft</a>. It has
since been &quot;embraced and extended&quot; to the point that it had to
be reverse engineered to work. Some documentation on this effort is available
<a href="http://www.hypothetic.org/docs/msn/index.php">here</a>.</p>

<a name="Novell"></a>
<h2>Novell</h2>
<p>
 GroupWise Messenger is a cross-platform instant messaging system
 designed for the corporate marketplace. It leverages Novell eDirectory
 for user authentication, user searching, and system administration.
 Conversations are secured using SSL encryption between the client and
 server. The protocol supports two-way conversations, multi-way
 conversations, presence information/updates for contacts, contact
 properties (based on attributes in the directory), custom status
 messages, auto replies, and privacy management.
</p>

<a name="oscar"></a>
<h2>Oscar</h2>
<p>
 Oscar is the official AIM protocol created by AOL, and all official AIM
 clients use Oscar. Oscar is a closed protocol, unlike TOC; AOL has never
 released any specifications on Oscar, which means that all of our
 knowledge about Oscar comes from reverse engineering. The libfaim authors
 have been working on this for nearly three years. Purple first featured the
 ability to use libfaim in 0.9.7, though it was never worked on enough to
 be usable; it wasn't until 0.9.16 when it was somewhat usable, and 0.9.20
 when it was able to be used without special compilation options. Oscar is
 now the protocol that most Purple users use (as witnessed by the outcries on
 Slashdot when it stops working).
</p>
<p>
 Oscar is a TCP-based protocol. It is binary, which means that commands are
 stored in integers rather than in character strings. This makes it
 considerably easier to extend the protocol when you want to add new
 features. For any successful signon, there are at least three connections
 made: first to the Authorizor, which validates your username and password;
 then to the BOS (Basic Oscar Service), the main connection to which and
 from which you send and receive messages; and ChatNav (Chat Navigation),
 which allows you to create and join chat rooms. In addition, each chat
 room that you join is another TCP connection. Finally, each file transfer
 and direct connection you make is another TCP connection, but these never
 pass through AOL. There are two ways of sending your password, XOR-based
 and MD5-based; libfaim currently uses the MD5 authentication. All messages
 are routed through AOL, except for direct connections.
</p>
<p>
 Because Oscar is not an open protocol, Purple doesn't support a lot of the
 features that it offers. It does, however, support all of the features
 that TOC offers, as well as several that TOC does not offer (including the
 ability to get users' away messages, and searching for users by email
 address). Also, AOL has a habit of blocking unofficial clients from
 connecting using Oscar, which means that occasionally Purple does get
 blocked. However, Purple itself has never been targeted to be blocked - it
 has always just been incidental while AOL blocked another service (Odigo
 and Jabber come to mind).
</p>
<p>
 The <a href="http://en.wikipedia.org/wiki/OSCAR_protocol">wikipedia
 article on Oscar</a> links to assorted documentation of the protocol.
</p>

<a name="Sametime"></a>
<h2>Sametime</h2>
<p>
 Support for Lotus Sametime in Purple is available via an optional add-on
 package from the Meanwhile project, at <a
 href="http://meanwhile.sourceforge.net">http://meanwhile.sourceforge.net</a>.
</p>
<p>
 A Sametime (nee Virtual Places) client uses a binary protocol built around a
 single TCP connection to a Sametime server, communicating in what is
 known as the Master protocol. Communication between clients is
 facilitated by a number of services. Services provide functionality such
 as messaging, conferencing, presence subscription, server-side storage
 of settings, and a user directory. Each service requires the client to
 handle a different sub-protocol over one or more Channels.
</p>
<p>
 Sametime communities must be explicitly connected. If two communities
 are not explicitly connected, there is no way for a user on one
 community to communicate with a user of another community.
</p>
<p>
 The Sametime protocol was originally published in draft form to the IETF
 as part of the IMPP working group. The "houri-draft" has long since
 expired, but was mostly a correct reference to the Master protocol and
 some sub-protocols. A copy is available <a href="http://meanwhile.sourceforge.net/docs/draft-houri-sametime-community-client-00.txt">here</a>.
</p>
<p>
 The Meanwhile project is not a product of IBM or Lotus, and is therefore
 neither endorsed nor supported by either.
</p>

<a name="toc"></a>
<h2>TOC</h2>
<p>
 TOC was created by AOL to allow unofficial clients to connect to the AIM
 service. TOC was originally used by TiK, an AIM client written in tcl/Tk.
 Purple was one of the first clients to make use of the protocol - if it
 hadn't been for TOC, Purple wouldn't exist.
</p>
<p>
 TOC is a TCP-based protocol. It is ASCII-based, but uses a 6-byte binary
 header for length information. There is usually only one connection made
 in a normal session; however, a connection is made for every file transfer
 that takes place, but these are direct connections, and so never go
 through AOL. Passwords are sent using a weak encryption algorithm. All
 messages are routed through AOL, except for when a direct connection is
 requested - however there are no TOC clients that support direct IMs (that
 I know of).
</p>
<p>
 Because TOC is not the official AIM protocol, and no official client has
 ever used TOC (with the exception of TiK), in addition to not keeping up
 with the official protocol in terms of functionality, TOC's usability has
 actually deteriorated since it was released. It does not support new
 features that official clients support, including the ability to request a
 file transfer (it can accept a request made by an official client though),
 and the ability to view users' away messages without relying on an
 auto-reply. It has also lost capabilities, such as searching for users by
 email address.
</p>

<a name="yahoo"></a>
<h2>Yahoo!</h2>
<p>This section has not yet been written.</p>

<a name="zephyr"></a>
<h2>Zephyr</h2>
<p>
 Zephyr, or more completely the Project Athena Zephyr notification system,
 is an IM system designed by MIT. It is founded on basic Unix principles -
 simple applications that do one thing very well. A normal Zephyr client
 installation normally consists of eight or nine clients - each which serve
 a different purpose (one for receiving messages, one for sending them, one
 for searching for online users, etc.). Purple attempts to integrate the
 functionality of several of these clients (most of which are
 console-based) into one GUI client. Zephyr is based on a library that is
 distributed in the Zephyr source, which is what Purple uses to connect to
 the Zephyr service.
</p>
<p>
 Zephyr is a UDP-based protocol, and in order to keep messages reliable, it
 has a "host manager". Every machine that acts as either a client or a
 server for Zephyr must be running one and only one instance of the Zephyr
 host manager (zhm). Purple does not provide this, so in order to use Zephyr
 in Purple, you must make sure that zhm is properly set up and configured
 first. The Zephyr library gets your username from getuid() or your Kerberos
 name - meaning that the username and password you tell Purple are ignored. 
 It also means that you can only use Purple to connect to Zephyr once (you 
 actually can connect to it multiple times, but the more often you connect,
 the more of a drain on server resources you are).
</p>

