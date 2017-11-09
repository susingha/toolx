<?php

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);






$username = $_GET["username"];
$available = '1';


$query = "SELECT username FROM usernames WHERE username = '" . $username . "'";
$result = mysql_query($query);

while($row = mysql_fetch_array($result))
{
	if($row['username'] == $username)
	{
		$available = '0';
		break;
	}
}


echo $available;

mysql_close();
?>