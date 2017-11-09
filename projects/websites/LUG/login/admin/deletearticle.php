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

$query = "SELECT filename FROM articles WHERE no = '" . $no . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	$filename = $row["filename"];
}

if($loggedin)
{
// delete the entry for the file from the database
	$query = "DELETE FROM articles WHERE no = '" . $no . "'";
	$result = mysql_query($query);
// now delete the file physically
	unlink("../../articles/" . $filename);
	if(!$result)
	{
		echo "Unable to delete. Please try later or report to administrator";
	}
	else
	{
		mysql_close();
		header('Location:moderate.php');
	}
}



?>