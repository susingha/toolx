<?php
session_start();

session_register("username");
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









///////////////// attempting to retrieve the maxreg and deciding the registration no.

$maxreg = "";
$f=fopen("./maxreg","r+") or exit("Unable to open file for reading!");
while (!feof($f))
{
	$x=fgetc($f);
	$maxreg = $maxreg . $x;
}
fclose($f);

$maxreg = $maxreg + 1;
$regno = substr($yrjoin, strlen($yrjoin)-2) . padzero($maxreg, 6);

$f=fopen("./maxreg","w") or exit("Unable to open file for writing!");
fwrite($f, $maxreg);
fclose($f);


session_register("rand");
$rand = rand(1000,9999);







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
	//redirect to useranametaken page
	header('Location:../?page=registration/usernametaken.html');
}
else
{
	//insert and rediredt to homepage
	$result = mysql_query("INSERT INTO usernames VALUES('$username','$password','1','$name','$from','$email','$phone','$linux','$date')");
	$result = mysql_query("INSERT INTO internal VALUES('$username','$dept','$roll','$yrjoin','$regno','0','','0')");
	$result = mysql_query("INSERT INTO secretcodes VALUES('$username','$rand','reg')");

	mysql_close();
	header('Location:../?page=registration/welcome_internal.php');

}



mysql_close();


function padzero($num, $padlen)
{
	$len = strlen($num);	
	if($len < $padlen)
	{
		$pad = "";
		for($i = 1;$i <= $padlen - $len;$i++)
		{
			$pad = $pad . "0";
		}
	}
return $pad . $num;
}

?>