<?php
session_start();
if($loggedin < 3 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page.</center>";
	return;
}


$usr = $_GET["usr"];
$regn = $_GET["regn"];

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);



$query = "SELECT username, regno FROM internal WHERE username = '" . $usr . "' AND regno = '" . $regn . "'";
$result = mysql_query($query);

if(mysql_num_rows($result) == 0)
{	
	echo "Unable to complete request. Username and Registration No did not match in database.";
	mysql_close();
	return;
}



$query = "UPDATE internal SET admin = 1 WHERE username = '" . $usr . "' AND regno = '" . $regn . "'";
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
	header('Location:../home.php');
}
?>