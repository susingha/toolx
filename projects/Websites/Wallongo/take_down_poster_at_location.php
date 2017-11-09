<?php

$con = mysql_connect("127.0.0.1","root","root");
if (!$con) {
	die('Could not connect: ' . mysql_error());
	echo "could not connect<br>";
  }
mysql_select_db("wall_on_go", $con);

$in_user     = $_GET["in_u"];
$in_heading  = $_GET["in_h"];
$in_location = $_GET["in_l"];

$query = "DELETE FROM poster_table WHERE heading = '$in_heading' AND location_name = '$in_location' AND username = '$in_user';";
$result = mysql_query($query);

?>
