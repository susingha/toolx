<?php

$con = mysql_connect("127.0.0.1","root","root");
if (!$con) {
	die('Could not connect: ' . mysql_error());
	echo "could not connect<br>";
  }
mysql_select_db("wall_on_go", $con);

$t1 = $_GET["t1"];
$t2 = $_GET["t2"];
$t3 = $_GET["t3"];
echo "$t1<br>";
echo "$t2<br>";
echo "$t3<br>";

$query = "SELECT * FROM location_table";
$result = mysql_query($query);

while($row = mysql_fetch_array($result)) {
	echo $row["name"] . " (" . $row["latitude"] . ")(" . $row["longitude"] . ")<br>";
}

?>
