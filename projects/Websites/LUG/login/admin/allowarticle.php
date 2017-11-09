<?php
session_start();
if($loggedin < 3 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page.</center>";
	return;
}

$no = $_GET["no"];

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "UPDATE articles SET moderated = 1 WHERE no = '" . $no . "'";
$result = mysql_query($query);
if(!$result)
{
	echo "sorry unable to perform update database action. An error occurred";
	mysql_close();
	return;
}
else
{
	mysql_close();
	header('Location:moderate.php');
}
?>
