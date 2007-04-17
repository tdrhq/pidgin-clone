<?php
	require "inc/base.inc.php";

	setup_site(-1);

	if ($users->logged_in()) {
		$users->logout();
	}

	setup_site("Log Out");

	start_section("Log Out");
?>
<p>
 You are now logged out.
</p>
<p>
 You may <a href="login.php">login</a> when you want to feel special again.
</p>
<?php
	end_section();

	site_shutdown();
?>
