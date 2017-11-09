<?php
session_start();

$loggedin = 0;
$username = $_GET["u"];
$password = $_GET["p"];


$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT username, password, member FROM usernames WHERE username = '" . $username . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	if($username != "" && $row['username'] == $username && $password != "" && $row['password'] == $password)
	{
		$loggedin = 1;		
		session_register("loggedin");
		session_register("username");
		session_register("cookie");

		//now send two cookies containing username and a random number.
		setcookie("user",$username,time()+36000,"/");
		$cookie = rand(10000,99999) * 31 + 453;
		setcookie("rcook",$cookie,time()+36000,"/");

		if($row['member'])
		{
			$loggedin = 2;
			$query = "SELECT admin, regno FROM internal WHERE username = '" . $username . "'";
			$result = mysql_query($query);
			while($row = mysql_fetch_array($result))
			{
				if($row['admin'])
				{
					$loggedin = 3;
				}
				if($row['regno'] == $username)
				{
					session_register("regno");
					$regno = $row['regno'];
				}
			}
		}
	}
}


mysql_close();
echo $loggedin;
?>