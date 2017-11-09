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


$no = $_GET["no"];
$query = "SELECT * FROM articles WHERE no = '" . $no . "'";
$result = mysql_query($query);


?>


<center>
<table border="0" cellpadding="0" style="border-collapse: collapse; table-layout: fixed" width="95%" id="table1">
	<tr>
		<td>
<h3><a onclick="showpage('login/home.php');">My Home</a> : <a onclick="showpage('login/admin/moderate.php');">Moderate uploads</a> : Article</h3><br>
		
<?php
while($row = mysql_fetch_array($result))
{
	echo '<table width="100%"><tr><td align="left" valign="bottom"><h2>' . $row["topic"] . '</h2>' . $row["format"] . '</td><td align="right" valign="bottom"><a onclick=showpage("login/admin/allowarticle.php?no=' . $no . '")>Allow</a> | <a onclick=showpage("login/admin/deletearticle.php?no=' . $no . '")>Delete</a></td></tr></table><hr>';
	if($row["format"] == "html" || $row["format"] == "htm")
	{
	//include the file
	
	$f=fopen("../../articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);
	
	
	//include "../../articles/" . $row["filename"];
	}
	else if($row["format"] == "txt")
	{
	// show in text box
	//echo '<textarea rows="30" cols="58">';
	echo '<pre>';
	$f=fopen("../../articles/" . $row["filename"],"r") or exit("Unable to open file!");
	while (!feof($f))
	{
		$x=fgetc($f);
		echo $x;
	}
	fclose($f);
	
	//include "../../articles/" . $row["filename"];
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

echo '<table width="100%"><tr><td align="right" valign="top"><a onclick=showpage("login/admin/allowarticle.php?no=' . $no . '")>Allow</a> | <a onclick=showpage("login/admin/deletearticle.php?no=' . $no . '")>Delete</a></td></tr></table>';
?>		


		</td>
	</tr>
</table>
</center>