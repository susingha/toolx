<?php
session_start();

session_register("username");
$username =  $_POST["username"];
$password = $_POST["password"];
$name = $_POST["name"];
$from = $_POST["from"];
$dd = $_POST["dd"];
$mm = $_POST["mm"];
$yyyy = $_POST["yyyy"];
$dob = $dd . "-" . $mm . "-" . $yyyy;
$email = $_POST["email"];
$phone = $_POST["phone"];
$inst = $_POST["inst"];
$spec = $_POST["spec"];
$other = $_POST["other"];
$linux = $_POST["linux"];




$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


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




if($available == 0)
{
	//rediredt to useranametaken page
	header('Location:../?page=registration/usernametaken.html');
}
else
{
	//insert and rediredt to homepage
	$result = mysql_query("INSERT INTO usernames VALUES('$username','$password','0','$name','$from','$email','$phone','$linux','1234')");
	$result = mysql_query("INSERT INTO external VALUES('$username','$dob','$inst','$spec','$other')");
	header('Location:../?page=registration/welcome_external.php');

}

mysql_close();

?>