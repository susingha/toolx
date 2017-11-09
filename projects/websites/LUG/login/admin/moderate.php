<?php
session_start();
if($loggedin < 3 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page.</center>";
	return;
}


$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);
?>






<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1"><tr><td>
<h3><a onclick="showpage('login/home.php');">My Home</a> : Moderate uploads</h3>
<h1>Welcome to moderator page.</h1>
</td></tr></table>


<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1"><tr><td>
<h2>Papers to moderate.</h2>
<br>
<?php

$query = "SELECT * FROM papers WHERE moderated = 0";
$result = mysql_query($query);
if(mysql_num_rows($result) == 0)
{
echo "<b>No papers to moderate.<b>";
}
?>
</td></tr></table>




<table border="1" cellpadding="8" style="border-collapse: collapse" width="90%" id="table1" cellspacing="0" bordercolor="#C0C0C0">
<?php

while($row = mysql_fetch_array($result))
{
	if($row["topic"] == "")
	{
		$topic = $row["filename"];
	}
	else
	{
		$topic = $row["topic"];
	}
	
echo '


	<tr>
		<td align="left" valign="top">
		<a onclick=showpage("login/admin/paper.php?no=' . $row['no'] . '")>'. $topic .'</a> | '. $row['format'] .'
		<br>
		'.$row["catch"].'
		</td>
		<td align="left" valign="top" width="110">
		uploaded by
		<br>
		<b><a onclick=showpage("login/profile.php?username=' . $row['username'] . '")>'.$row["username"].'</a></b>
	
		<br>
		'.$row["date"].'
		</td>
		<td align="left" valign="top" width="150">
		Paper no : 
		'.$row["no"].'
		<br>
		Rating : 
		'.$row["rating"].'
		<br>
		<table border="0" cellpadding="0" style="border-collapse: collapse" width="100%" id="table2">
			<tr>
				<td align="left">
				<a onclick=showpage("login/admin/allowpaper.php?no=' . $row['no'] . '")>Allow</a>
				</td>
				<td align="left">
				<a onclick=showpage("login/admin/paper.php?no=' . $row['no'] . '")>View</a>
				</td>
				<td align="left">
				<a onclick=showpage("login/admin/deletepaper.php?no=' . $row['no'] . '")>Delete</a>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	
';
}

?>
</table>




<br>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1"><tr><td>
<h2>Articles to moderate.</h2>
<br>
<?php

$query = "SELECT * FROM articles WHERE moderated = 0";
$result = mysql_query($query);
if(mysql_num_rows($result) == 0)
{
echo "<b>No articles to moderate.<b>";
}

?>
</td></tr></table>




<table border="1" cellpadding="8" style="border-collapse: collapse" width="90%" id="table1" cellspacing="0" bordercolor="#C0C0C0">

<?php

while($row = mysql_fetch_array($result))
{
	if($row["topic"] == "")
	{
		$topic = $row["filename"];
	}
	else
	{
		$topic = $row["topic"];
	}
	
echo '


	<tr>
		<td align="left" valign="top">
		<a onclick=showpage("login/admin/article.php?no=' . $row['no'] . '")>'. $topic .'</a> | '. $row['format'] .'
		<br>
		'.$row["catch"].'
		</td>
		<td align="left" valign="top" width="110">
		uploaded by
		<br>
		<b><a onclick=showpage("login/profile.php?username=' . $row['username'] . '")>'.$row["username"].'</a></b>
		<br>
		'.$row["date"].'
		</td>
		<td align="left" valign="top" width="150">
		Article no : 
		'.$row["no"].'
		<br>
		Rating : 
		'.$row["rating"].'
		<br>
		<table border="0" cellpadding="0" style="border-collapse: collapse" width="100%" id="table2">
			<tr>
				<td align="left">
				<a onclick=showpage("login/admin/allowarticle.php?no=' . $row['no'] . '")>Allow</a>
				</td>
				<td align="left">
				<a onclick=showpage("login/admin/article.php?no=' . $row['no'] . '")>View</a>
				</td>
				<td align="left">
				<a onclick=showpage("login/admin/deletearticle.php?no=' . $row['no'] . '")>Delete</a>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	
';
}


mysql_close();
?>

</table>
</center>