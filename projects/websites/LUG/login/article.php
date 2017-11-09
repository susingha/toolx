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
<h3><a onclick="showpage('login/home.php');">My Home</a> : <a onclick="showpage('login/showarticles.php');">Articles uploaded by me</a></h3>
<br>
		
<?php
while($row = mysql_fetch_array($result))
{
	echo '<h2>' . $row["topic"] . '</h2>' . $row["format"] . '<hr>';
	if($row["format"] == "html" || $row["format"] == "htm")
	{
	//read the file and echo
	
	$f=fopen("../articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);

	
	//include "../articles/" . $row["filename"];
	}
	else if($row["format"] == "txt")
	{
	// read the file and show in text box
	//echo '<textarea rows="30" cols="58">';
	echo '<pre>';
	$f=fopen("../articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);

	
	//include "../articles/" . $row["filename"];
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
}

mysql_close();
?>		


		</td>
	</tr>
</table>
</center>