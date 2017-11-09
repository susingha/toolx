<?php

$regno = $_GET["regno"];

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);

$query = "DELETE FROM usernames WHERE username = '" . $regno . "'";
	$result = mysql_query($query);
$query = "DELETE FROM internal WHERE username = '" . $regno . "'";
	$result = mysql_query($query);

header('Location:./show_all.php');
?>