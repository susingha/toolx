<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1"><tr><td>
<h3><a href=".">Home</a> : Papers</h3>
<br>
<h2>Papers</h2>
<br>
<?php

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT * FROM papers ORDER BY no DESC";
$result = mysql_query($query);
if(mysql_num_rows($result) == 0)
{
echo "<b>Nothing uploaded yet. <a onclick=showpage('login/home.php')>Be the first to upload.</a><b>";
mysql_close();
return;
}

?>
</td></tr></table>


<table border="1" cellpadding="8" style="border-collapse: collapse" width="90%" id="table1" cellspacing="0" bordercolor="#C0C0C0">

<?php

$cnt = $_GET["count"];
if($cnt == "") $cnt = -1;

while($row = mysql_fetch_array($result))
{	
	if($cnt-- == 0) break;
	
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
		<a href="?page=paper.php&no=' . $row['no'] . '">'. $topic .'</a> | '. $row['format'] .'
		<br>
		'.$row["catch"].'
		</td>
		<td align="left" valign="top" width="120">
		<b><a href="?page=login/profile.php&username='. $row["username"] .'">'. $row["username"] .'</a></b>
		<br>
		Rating : 
		'.$row["rating"].'
		<br>
		Moderated : 
		'.$moderated.'		
		</td>
	</tr>
	
';
}
mysql_close();
?>	

</table>
<br><h3>Came across sumthing gr8 about computers? <a onclick="showpage('login/home.php');">Upload a paper on it.</a></h3>
</center>

