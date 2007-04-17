<?
	$page->title = 'Passwords';
	require('inc/template.inc.php');
?>

<h1>Passwords</h1>

<p>Purple does not now and is not likely to encrypt the passwords in the 
<code>accounts.xml</code> file, nor is it likely to be encrypted in a 
future release. This is somewhat controversial in Windows, especially Windows 
98 due to its weak file protections, but that's the way things are.</p>

<p>The reasoning for this is multi-part.</p>

<p>Instant messaging is not very secure, and it's kind of pointless to 
spend a lot of time adding protections onto the fairly strong file 
protections of UNIX (our native platform) when the protocols themselves 
aren't all that secure.  The way to truly know who you are talking to is 
to use an encryption plugin on both ends (such as OTR or 
gaim-encryption), and use verified GPG keys.  Secondly, you shouldn't be 
using your instant messaging password for anything else.  While 
some protocols have decent password security, others are insufficient 
and some (like IRC) don't have any at all.</p>

<p>There are basically four approaches to password storage.</p>
<ol>
<li><b>Store a password(s) behind a password.</b> Basically this means that we 
require you to type in some passphrase as Purple starts in order to read the 
<code>accounts.xml</code> file, and, to be truely secure, require you to type it 
again if you write to it. Winicq does something very similar to this if 
you set it to its highest security settings.</li>
<li><b>Obscure a password.</b> This means we do something to store the 
password in some format other than plain text, but we automatically convert 
it for you.  This is security by obscurity, and is a Very Bad Thing&trade; in 
that it gives users a false sense of security.  A false sense that we (Purple 
developers) believe would be worse to have than to let informed users 
deal with the password issue themselves.  Consider that a naive user 
might think that it is safe to share his or her accounts.xml, because 
the passwords are "encrypted".</li>
<li><b>Store the password in plain text and control access to the file.</b> 
This is what Purple does: the password is in <code>accounts.xml</code> in plain 
text, but the file itself is only readable by its owner.  We allow the user 
to determine under what conditions sensitive files should be opened (if at all), 
and what constitutes a breach of security.</li>
<li><b>Lastly, you can not store passwords at all.</b> This is Purple's default, 
and by far the most secure of all of the options.</li>
</ol>

<p>If you really wanted to, you could write a script to wrap Purple that 
would decrypt <code>accounts.xml</code> and re-encrypt it when Purple exits.
You wouldn't be able to encrypt it while Purple is running,  because Purple 
writes to <code>accounts.xml</code> for things like info change.  This would 
minimize your exposure time unless (like me) you run Purple nearly 24/7. 
Personally, I feel that on any decent operating system, if someone can get to 
your files you should either be able to trust the person to not touch 
them, or you shouldn't be storing sensitive information there at all.</p>
