
<?php

$username = $_POST["username"];
$password = $_POST["password"];
$name     = $_POST["name"];
$sbuid    = $_POST["sbuid"];
$email    = $_POST["email"];
$desc     = $_POST["desc"];
$why      = $_POST["why"];
$kind     = $_POST["kind"];
$many     = $_POST["many"];
$order    = $_POST["order"];
$what     = $_POST["what"];
$time     = $_POST["time"];
$prefer   = $_POST["prefer"];

//echo $username . "<br>";
//echo $password . "<br>";
//echo $name . "<br>";
//echo $sbuid . "<br>";
//echo $email . "<br>";
//echo $desc . "<br>";
//echo $why . "<br>";
//echo $kind . "<br>";
//echo $many . "<br>";
//echo $order . "<br>";
//echo $what . "<br>";
//echo $time . "<br>";
//echo $prefer . "<br>";

$con = mysql_connect("mysql5.000webhost.com","a5853978_super","super123");
if (!$con)
{
    die('Could not connect: ' . mysql_error());
    echo "could not connect<br>";
}
mysql_select_db("a5853978_account", $con);
echo mysql_error();

$query = "INSERT INTO accounts VALUES('$username',
				      '$password',
                                      '$name',
				      '$sbuid',
				      '$email',
				      '$desc',
				      '$why',
				      '$kind',
				      '$many',
				      '$order',
				      '$what',
				      '$time',
				      '$prefer',
				      0,
				      0,
				      0,
				      0);";

$result = mysql_query($query);

//$query = "SELECT * from accounts";
//$result = mysql_query($query);
//while($row = mysql_fetch_array($result))
//{
//    echo $row['a_username'];
//    echo $row['a_password'];
//    echo $row['a_name'];
//    echo $row['a_use'];
//    echo $row['a_username'];
//    echo $row['a_username'];
//    echo $row['a_username'];
//    echo $row['a_username'];
//}

mysql_close();

?>

<br><br><br><br><br><br><br><br>
<center>
Thank you for your registration. Please try <a href="login.php">logging in</a> with your username and password.
</center>





