<?php

$username = $_POST["regno"];
$password = $_POST["dd"] . $_POST["mm"];
$regno = $_POST["regno"];




$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$result1 = mysql_query("INSERT INTO usernames (username, password, member) VALUES('$username','$password','1')");
$result2 = mysql_query("INSERT INTO internal (username, regno, paid, admin) VALUES('$username','$regno','1','0')");

mysql_close();
header('Location:./fill_in_details.html');


?>
