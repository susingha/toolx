<?php
session_start();


$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$no = $_GET["no"];
$query = "SELECT * FROM articles WHERE no = '" . $no . "'";
$result = mysql_query($query);

?>


<center>
<table border="0" cellpadding="0" style="border-collapse: collapse; table-layout: fixed" width="95%" id="table1">
	<tr>
		<td>
		
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

	echo '<table width="100%"><tr><td align="left" valign="bottom"><h3><a href=".">Home</a> : <a href="./?page=showarticles.php">Papers</a></h3><br><h2>' . $topic . '</h2>' . $row["format"] . '</td><td align="right" valign="bottom"><h3>Moderated : '. $moderated .' : Rating : '. $row["rating"] .'<br>Submitted by <i>'. $row["username"] .'</i> on '. $row["date"] .'</h3></td></tr></table><hr>';
	if($row["format"] == "html" || $row["format"] == "htm")
	{
	//include the file
	
	$f=fopen("articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);
	
	
	//include "articles/" . $row["filename"];
	}
	else if($row["format"] == "txt")
	{
	// show in text box
	//echo '<textarea rows="30" cols="58">';
	echo '<pre>';
	$f=fopen("articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);
	
	//include "articles/" . $row["filename"];
	echo '</pre>';
	//echo "</textarea>";
	}
	else 
	{
	// show as link
	echo '<a href="articles/' . $row['filename'] . '">';
if($row["topic"] != "")
	echo $row["topic"];
else
	echo $row['filename'];
	echo '</a><br>';
	echo $row["catch"];
	
	}
echo '<table width="100%"><tr><td align="left" valign="bottom"></td><td align="right" valign="bottom"><h3>Submitted by <i>'. $row["username"] .'</i> on '. $row["date"] .'<br>Moderated : '. $moderated .' : Rating : '. $row["rating"] .'</h3></td></tr></table>';
}


mysql_close();
?>		


		</td>
	</tr>
</table>
</center>