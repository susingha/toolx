<?php

session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}


if($loggedin <= 1)
{
	echo '<br>Sorry for member login only';
	return;
}
$regno = $username;

$username =  $_POST["username"];
$password = $_POST["password"];
$name = $_POST["name"];
$from = $_POST["from"];

$email = $_POST["email"];
$phone = $_POST["phone"];

$dept = $_POST["dept"];
$roll = $_POST["roll"];
$yrjoin = $_POST["yrjoin"];
$linux = $_POST["linux"];

$date = date('d M Y');




$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "UPDATE usernames SET username = '$username', password = '$password', name = '$name', frm = '$from', email = '$email', phone = '$phone', linux = '$linux', regdate = '$date' WHERE username = '$regno'";
$result = mysql_query($query);

$query = "UPDATE internal SET username = '$username', dept = '$dept', roll = '$roll', yrjoin = '$yrjoin' WHERE username = '$regno'";
$result = mysql_query($query);

setcookie("user",$username,time()+36000,"/");
header('Location:../../?page=login/home.php');

?>