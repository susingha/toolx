<?php

$con = mysql_connect("127.0.0.1","root","root");
if (!$con) {
	die('Could not connect: ' . mysql_error());
	echo "could not connect<br>";
  }
mysql_select_db("wall_on_go", $con);

$in_user      = $_GET["in_u"];
$in_latitude  = $_GET["in_la"];
$in_longitude = $_GET["in_lo"];

$radius = 0.0030;

$in_latitude_min = $in_latitude - $radius;
$in_latitude_max = $in_latitude + $radius;
$in_longitude_min = $in_longitude - $radius;
$in_longitude_max = $in_longitude + $radius;


$query = "SELECT * FROM poster_table WHERE EXISTS
              (SELECT * FROM location_table WHERE
                 latitude > ('$in_latitude_min') and latitude < ('$in_latitude_max')
                 and longitude > ('$in_longitude_min') and longitude < ('$in_longitude_max')
                 and location_table.name = poster_table.location_name);";
$result = mysql_query($query);
$i = 0;

while($row = mysql_fetch_array($result)) {
        $i = $i + 1;
	echo "Poster" . $i . ": <b>" . $row["heading"] . "</b> " . $row["body"] . " at <b>" . $row["location_name"] . "</b> by <i>" . $row["username"] . " -- <u>reply</u>, <u>fav</u></i><br>";
}

?>
