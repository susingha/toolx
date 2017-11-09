

<center>
<br><br>
<?php
$username = $_POST["username"];
$password = $_POST["password"];
$crack = $_GET["crack"];

// echo $username . "<br>";
// echo $password . "<br>";

$authenticated = false;
$con = mysql_connect("mysql5.000webhost.com","a5853978_super","super123");
if (!$con)
{
    die('Could not connect: ' . mysql_error());
    echo "could not connect<br>";
}
mysql_select_db("a5853978_account", $con);
echo mysql_error();

$query = "SELECT a_username, a_password, a_logins, a_failed, a_attempts, a_cracked FROM accounts WHERE a_username = '" . $username . "'";
$result = mysql_query($query);

while($row = mysql_fetch_array($result)) {
    if($row['a_password'] == $password && $row['a_username'] == $username)
    $authenticated = True;
    $logins = $row['a_logins'];
    $failed = $row['a_failed'];
    $attempts = $row['a_attempts'];
    $cracked = $row['a_cracked'];
    break;
}

//echo "<hr>";
//echo $crack;
//echo "<hr>";

if ($crack) {
    $attempts++;
} else {
    $logins ++;
}



if($authenticated) {
    if($crack) {
	echo "You have cracked the password of " . $username . ". <a href='index.php'>Startover</a>";
	$cracked++;
    } else {
	echo $username . ". You are authenticated. <a href='index.php'>Startover</a>";
    }
} else {
    if($crack) {
	echo "You could not crack the password of " . $username . " <a href='login.php?crack=1&username=" . $username . "'>Try Again</a>";
    } else {
	echo $username . ". Authenticated failed. <a href='login.php'>Try Again</a>";
	$failed++;
    }
}


$query = "UPDATE accounts SET a_logins = '$logins', a_failed = '$failed', a_attempts = '$attempts', a_cracked = '$cracked' WHERE a_username = '$username'";
$result = mysql_query($query);


?>

<br><hr>
Crack someone else's password:
<br><br>
<table border="0" cellpadding="5" style="border-collapse: collapse;" width="60%">
<?php

$query = "SELECT a_username, a_password, a_logins, a_failed, a_attempts, a_cracked FROM accounts;";
$result = mysql_query($query);

while($row = mysql_fetch_array($result)) {
    $username = $row['a_username'];
    $logins = $row['a_logins'];
    $failed = $row['a_failed'];
    $attempts = $row['a_attempts'];
    $cracked = $row['a_cracked'];

    echo '<tr><td style="vertical-align:left;">';
    echo '<a href="login.php?crack=1&username=' . $username . '">' . $username . '</a> </td><td style="vertical-align:right;"> ' . $failed . ' failed / ' . $logins . ' logins. ' . $cracked . ' cracked / ' . $attempts. '  attempts';
    echo '</td></tr>';
}

mysql_close();
?>
</table>
<br>
</center>
