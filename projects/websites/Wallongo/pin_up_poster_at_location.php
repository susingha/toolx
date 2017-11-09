<?php

$con = mysql_connect("127.0.0.1","root","root");
if (!$con) {
	die('Could not connect: ' . mysql_error());
	echo "could not connect<br>";
  }
mysql_select_db("wall_on_go", $con);

$in_heading  = $_GET["in_h"];
$in_body     = $_GET["in_b"];
$in_location = $_GET["in_l"];
$in_latitude  = $_GET["in_la"];
$in_longitude = $_GET["in_lo"];
$in_user     = $_GET["in_u"];

$query = "INSERT INTO poster_table VALUES ('$in_heading', '$in_body', '$in_location', '$in_user');";
$result = mysql_query($query);
$query = "INSERT INTO location_table VALUES ('$in_location', '$in_latitude', '$in_longitude');";
$result = mysql_query($query);

echo $in_location . " added at " . $in_latitude . ", " . $in_longitude;

?>
