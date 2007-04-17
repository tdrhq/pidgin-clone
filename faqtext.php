<html>
<body>
<table>
<?php
	$faq = file("./faq.txt");
	$temp='';
	$qa=0;
	for($i=0; $i < sizeof($faq); $i=$i+2)
	{
		if($qa % 2 == 0) // is even
		{
			?>
			<tr>
			<td valign="top"><img src="/images/q.png" width="20" height="32" border="0" alt="Q." /></td>
			<td valign="top"><br /><font face="Verdana, Tahoma, Arial, Helvetica" size="-1">
			<?php
				$temp = substr($faq[$i],3);
				print($temp);
			?>
			</font></td>
			</tr>
			<?php
		}
		if(!($qa % 2 == 0))  //is odd
		{
			?>
			<tr>
			<td valign="top"><img src="/images/a.png" width="20" height="27" border="0" alt="A." /></td>
			<td valign="top"><br /><font face="Verdana, Tahoma, Arial, Helvetica" size="-1">
			<?php
				$temp = substr($faq[$i],3);
				print($temp);
			?>
			</font></td>
			</tr>
			<?php
		}
		$qa++;
	}
?>
</table>
</body>
</html>

