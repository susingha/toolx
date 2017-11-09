<?php
session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}


if($loggedin)
{
// decide the fields
	$max = "";
	$f=fopen("./articlemax","r+") or exit("Unable to open file for reading!");
	while (!feof($f))
	{
		$x=fgetc($f);
		$max = $max . $x;
	}
	fclose($f);
	$max = $max + 1;
	$no = padzero($max, 5);
	$f=fopen("./articlemax","w") or exit("Unable to open file for writing!");
	fwrite($f, $max);
	fclose($f);

	$topic = $_POST["topic"];
	$catch = $_POST["catch"];
	$content = $_POST["content"];

	$date = date('d M Y');
	$format = "txt";
	$filename = $topic . $no . ".txt";


// now write the file;
	$f=fopen("../articles/" . "$filename","w") or exit("Unable to open file for writing!");
	fwrite($f, $content);
	fclose($f);



//open  the connection and insert
	$con = mysql_connect("localhost","root","");
	if (!$con)
	  {
	  die('Could not connect: ' . mysql_error());
	  echo "could not connect<br>";
	  }
	mysql_select_db("lug_nitdgp", $con);


	$result = mysql_query("INSERT INTO articles VALUES('$no','0','$topic','$catch','$username','$date','$filename','$format','0')");
	mysql_close();

//now redirect to homepage
	header('Location:home.php');
}






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