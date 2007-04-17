<?
	$page->title = 'MSN Information';
	require('inc/template.inc.php');
?>

<h1>MSN Information - Will we be blocked?</h1>

<p>
<b>The following was written by a Pidgin developer, Christian Hammond, who is
responsible for much of Pidgin's MSN code.  Christian has been following the
situation closely, and below is his summary of it.  However, his views are
not necessarily the views of all the Pidgin developers and should not be taken
as an "official" statement.  Rob and I are presently looking into the best
course of action and will be sure to keep you up to date.  We both remain
confident (as does Christian) that Pidgin will have no problems connecting to
MSN Messenger in the near future.<br/>
--Sean Egan, 23 August, 2003</b>
</p>

<p>
 A lot of people have been asking about whether MS's latest decisions
 regarding MSN will affect us. Here's the general information on what
 they're doing.
</p>
<p>
 MS has decided that everyone should upgrade to MSN version 5.0 or higher.
 MSN v5 is the first to use their MSN Protocol v8, which is the first to
 use SSL for login. Now, we know how to login over this. This is not hard,
 and we will be supporting MSNP9, the latest version of the protocol, soon.
</p>
<p>
 However, upgrading to this does not guarantee us anything. Whereas
 previously, Microsoft has let third party clients connect, they now
 require a license for doing so. They still encourage clients to connect
 to their network, so with any luck, we can work something out. If not,
 people may find a way to connect anyway, but the legalities of this
 are pretty obvious. Key words: "Intellectual Property" and "DMCA."
</p>
<p>
 We cannot give a yes or a no to the question, "Will Pidgin continue to work
 after October 15th?" (Which, btw, is the day that third party clients
 not using MSNP8 or higher will be unable to connect). We will do our best,
 but if it comes down to it, we will drop MSN support. If you have a lot
 of friends on MSN, perhaps it's the time to migrate them. Get them to
 install Pidgin ;) Then they can continue to talk to their MSN friends, but
 move to Jabber or another protocol.
</p>

