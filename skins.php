<?
	$page->title = 'Skins';
	require('inc/template.inc.php');
?>

<h1>Skins</h1>

<p>Skins are not now and will never be in Pidgin's UI. GTK themes,
however, are supported, as per the Pidgin FAQ. This confuses many users, so an
attempt at explanation follows.</p>

<p>Themes allow you to change font size, font face, the color of different
elements of the UI, and sometimes their shape. All of this is perfectly
reasonable, and encompasses the range of configuration that <i>most</i> users are
requesting. Skins go beyond this. Skins allow you to move and possibly supress
different elements of the UI that the programmers have added for specific
reasons.</p>

<p>In so doing, skins create a support nightmare. The developer, talking to the
user, cannot be sure that the user sees the same interface. Is it a bug that
the user cannot see how to change font size, or is the user's skin suppressing
that widget? This is an unacceptable situation.</p>

<p>Conversely, while themes allow some sillyness, for instance setting the
background to the same color as the font, the results are relatively easily
foreseen and avoided by theme authors. Further, by using the existing GTK theme
mechanism, we gain two advantages:</p>

<ol>
<li>We don't have to write it ourselves.  Were we to write our own mechanism, we
would be adding overhead in both the run time and the time necessary to debug.</li>
<li>We have a built in way of debugging theme problems, by checking if the same
bug exists with other GTK programs.</li>
</ol>

<p>Lastly, since GTK themes only allow acceptable modifications, we do not have
to worry that a user isn't seeing part of the UI because it is either showing
or not showing <i>because of our code</i>.</p>
