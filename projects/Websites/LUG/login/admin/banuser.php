<?php
session_start();
if($loggedin < 3 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page.</center>";
	return;
}


$usr = $_GET["usr"];

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT username, member FROM usernames WHERE username = '" . $usr . "'";
$result = mysql_query($query);

if(mysql_num_rows($result) == 0)
{	
	echo "Please check username. User ". $usr ." does not exist in database.";
	mysql_close();
	return;
}

while($row = mysql_fetch_array($result))
{
	$query1 = "DELETE FROM usernames WHERE username = '" . $usr . "'";
	if($row["member"])
	{
		$query2 = "DELETE FROM internal WHERE username = '" . $usr . "'";
		if(!$row["paid"])
		{
			$query3 = "DELETE FROM secretcodes WHERE username = '" . $usr . "'";
		}
	}
	else
	{
		$query2 = "DELETE FROM external WHERE username = '" . $usr . "'";
		$query3 = "";
	}
}


$result1 = mysql_query($query1);
$result2 = mysql_query($query2);
if($query3 != "")
$result3 = mysql_query($query3);


if(!$result1)
{
	echo "Unable to delete user from main database. An error occurred";
	mysql_close();
	return;
}
if(!$result2)
{
	echo "Unable to delete user from other database. An error occurred";
	mysql_close();
	return;
}

mysql_close();
header('Location:../home.php');

?>