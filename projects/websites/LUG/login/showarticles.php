<?php
session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}
?>

<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1"><tr><td>
<h3><a onclick="showpage('login/home.php');">My Home</a> : Articles uploaded by me</h3>
<br>
<h2>Articles uploaded by me ...</h2>
<br>
<?php

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT * FROM articles WHERE username = '" . $username . "'";
$result = mysql_query($query);
if(mysql_num_rows($result) == 0)
{
echo "<b>Nothing uploaded yet. Please upload an article.<b>";
mysql_close();
return;
}

?>
</td></tr></table>



<table border="1" cellpadding="8" style="border-collapse: collapse" width="90%" id="table1" cellspacing="0" bordercolor="#C0C0C0">

<?php

while($row = mysql_fetch_array($result))
{
	if($row["moderated"])
	{
		$moderated = "yes";
	}
	else
	{
		$moderated = "no";
	}
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
		<a onclick=showpage("login/article.php?no=' . $row['no'] . '")>'. $topic .'</a> | '. $row['format'] .'
		<br>
		'.$row["catch"].'
		</td>
		<td align="left" valign="top" width="120">
		Rating : 
		'.$row["rating"].'
		<br>
		Moderated : 
		'.$moderated.'
		<br>
		<table border="0" cellpadding="0" style="border-collapse: collapse" width="100%" id="table2">
			<tr>
				<td align="left">
				<a onclick=showpage("login/article.php?no=' . $row['no'] . '")>View</a>
				</td>
				<td align="left">
				<a onclick=showpage("login/deletearticle.php?no=' . $row['no'] . '")>Delete</a>
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

