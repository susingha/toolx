<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="95%">
<tr><td>


<table border="0" cellpadding="5" style="border-collapse: collapse" width="100%" id="table3" bgcolor="#FCFCFC">
	<tr>
		<td align="center" valign="top"><img src="images/kde.jpg" width="300" height="200" border="1"  alt="Image / flash"></td>
		<td align="left" valign="top">
		Linux Users' Goup, NIT Durgapur.
		The main event by Linux Users Group is Mukti, and as its name suggests 
		it has been introduced to free the mind of the students of the so many 
		junks of Proprietory operating systems and softwares available in the 
		market, and come at rocketing prices.
		<br><br>
		<?php
		if($_SERVER['REMOTE_ADDR'] == "127.0.0.1"
		|| $_SERVER['REMOTE_ADDR'] == "210.212.5.146"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.135"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.139"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.140"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.146"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.149"
		|| $_SERVER['REMOTE_ADDR'] == "220.225.80.150")
			echo '<h3><a href="?page=registration/internal.php">Register as member (NIT-Dgp students)</a></h3>';
		else
			echo '<h3 style="color: #AAAAAA">link to internal registration has been blocked because probably you are out side the NIT-DGP network</h3>';
		?>
		<h3><a href="?page=registration/external.php">Register as user (outside NIT-Dgp network)</a></h3>
		</td>
	</tr>
</table>
<hr color="#C0C0C0" size="1" noshade>
<table width="100%" cellspacing="5" cellpadding="3" border="0">
	<tr id="mainmenu">
		<td width="30%" bgcolor="#9D3231">News Piece</td>
		<td width="35%" bgcolor="#9D3231">White Paper</td>
		<td width="35%" bgcolor="#9D3231">Article</td>
	</tr>
	
	
<?php

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);
?>


	<tr>
		<td bgcolor="#FCFCFC" align="left" valign="top">
		<font size="2">
		Participate in <b>CODECRACKER</b> A fully automated, <b><i>intraLAN C/C++ programming contest</i></b>. Logon to http://codecracker1 or http://codecracker2 Register ur team and get going.<br>
		<a href="?page=noticeboard.php#codecracker">Details</a>
		</font>
		</td>
		<td bgcolor="#FCFCFC" align="left" valign="top">
		<?php
		$query = "SELECT * FROM papers WHERE moderated = 1 ORDER BY no DESC";
		$result = mysql_query($query);

		if(mysql_num_rows($result) == 0)
			echo "Nothing uploaded yet. <a onclick=showpage('login/home.php')>Be the first to upload.</a>";
		else
		{
			$cnt = 1;
			if($cnt == "") $cnt = -1;

			while($row = mysql_fetch_array($result))
			{	
				if($cnt-- == 0) break;
				
				if($row["topic"] == "")				
					$topic = $row["filename"];				
				else				
					$topic = $row["topic"];
					
				echo '<h3><i><a href="?page=paper.php&no=' . $row['no'] . '">'. $topic .'</a></i></h3>

				<font size="2">'.$row["catch"].'</font>
				<br>
				<font size="2" color="#9D9D9D"><a href="?page=showpapers.php&count=5">More >></a></font>
				<hr color="#C0C0C0" size="1" noshade>'
				;

			}
		}
		?>

		</td>
		<td bgcolor="#FCFCFC" align="left" valign="top">
		
		
		<?php
		$query = "SELECT * FROM articles WHERE moderated = 1 ORDER BY no DESC";
		$result = mysql_query($query);

		if(mysql_num_rows($result) == 0)
			echo "Nothing uploaded yet. <a onclick=showpage('login/home.php')>Be the first to upload.</a>";
		else
		{
			$cnt = 1;
			if($cnt == "") $cnt = -1;

			while($row = mysql_fetch_array($result))
			{	
				if($cnt-- == 0) break;
				
				if($row["topic"] == "")				
					$topic = $row["filename"];				
				else				
					$topic = $row["topic"];
					
				echo '<h3><i><a href="?page=article.php&no=' . $row['no'] . '">'. $topic .'</a></i></h3>

				<font size="2">'.$row["catch"].'</font>
				<br>
				<font size="2" color="#9D9D9D"><a href="?page=showarticles.php&count=5">More >></a></font>
				<hr color="#C0C0C0" size="1" noshade>'
				;
	
			}
		}
		?>

		</td>
	</tr>
</table>


</td></tr>
</table>
</center>
