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



/////////////////////////////////// now decide the fields ///////////////////
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
	$date = date('d M Y');
	$format = $_POST["format"];
	$filename = getfilename($_FILES['uploadarticle']['name'], $no, $format);


// Where the file is going to be placed
$target_path = "../articles/";
$target_path = $target_path . $filename;

if(move_uploaded_file($_FILES['uploadarticle']['tmp_name'], $target_path))
{



	// now open the connection and enter the fields
	$con = mysql_connect("localhost","root","");
	if (!$con)
	  {
	  die('Could not connect: ' . mysql_error());
	  echo "could not connect<br>";
	  }
	mysql_select_db("lug_nitdgp", $con);

	$query = 'INSERT INTO articles VALUES("'.$no.'","0","'.$topic.'","'.$catch.'","'.$username.'","'.$date.'","'.$filename.'","'.$format.'","0")';
	$result = mysql_query($query);
	mysql_close();

	// all work done now redirect to a page giving out the informations
	header('Location:../?page=login/uploadfine.php&file=' . $_FILES['uploadarticle']['name'] . '&type=' . $_FILES['uploadarticle']['type'] . '&size=' . $_FILES['uploadarticle']['size']);
}
else
{
	header('Location:../?page=login/uploadfine.php&error=' . $_FILES['uploadarticle']['error']);
}


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


function getfilename($namestring, $num, $ext)
{
	$len = strlen($namestring);
	for($i = $len - 1; $namestring[$i] != "." && $i >= 0; $i--);
	if($i >= 0)
	{
		$ext = substr($namestring, $i + 1);
		$namestring = substr($namestring, 0, $i);
	}

	if($len > 40)
	{
		$namestring = substr($namestring, 0, 40);
	}
	if($ext == "php"
	|| $ext == "Php"
	|| $ext == "pHp"
	|| $ext == "PHp"
	|| $ext == "phP"
	|| $ext == "PhP"
	|| $ext == "pHP"
	|| $ext == "PHP")
	$ext = "txt";
return $namestring . $num . "." . $ext;
}


?>








