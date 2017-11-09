<?php

session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}

if($loggedin != 1)
{
	echo '<br><br>For member login only';
	return;
}
	
$password = $_POST["password"];

$name = $_POST["name"];
$from = $_POST["from"];
$dob = $_POST["dd"] . "-" . $_POST["mm"] . "-" . $_POST["yyyy"];

$email = $_POST["email"];
$phone = $_POST["phone"];

$inst = $_POST["inst"];
$spec = $_POST["spec"];
$others = $_POST["others"];
$linux = $_POST["linux"];



$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);

$query = "UPDATE usernames SET password = '$password', name = '$name', frm = '$from', email = '$email', phone = '$phone', linux = '$linux' WHERE username = '$username'";
$result = mysql_query($query);

$query = "UPDATE external SET dob = '$dob', inst = '$inst', spec = '$spec', others = '$others' WHERE username = '$username'";
$result = mysql_query($query);

header('Location:../?page=login/home.php');

?>