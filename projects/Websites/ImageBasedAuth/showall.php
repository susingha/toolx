

<center>
<br><br>
<?php

$authenticated = false;
$con = mysql_connect("mysql5.000webhost.com","a5853978_super","super123");
if (!$con)
{
    die('Could not connect: ' . mysql_error());
    echo "could not connect<br>";
}
mysql_select_db("a5853978_account", $con);
echo mysql_error();

?>

<?php





$query = "SELECT a_username, a_password, a_logins, a_failed, a_attempts, a_cracked FROM accounts;";
$result = mysql_query($query);

$i = 1;
while($row = mysql_fetch_array($result)) {

    $username = $row['a_username'];
    $password = $row['a_password'];
    $logins = $row['a_logins'];
    $failed = $row['a_failed'];
    $attempts = $row['a_attempts'];
    $cracked = $row['a_cracked'];



    echo '<table border="0" cellpadding="5" style="border-collapse: collapse;" width="60%">';
    echo '<tr><td style="vertical-align:left;">';
    echo $i++ . ' <a href="login.php?crack=1&username=' . $username . '">' . $username . '</a> </td><td style="vertical-align:right;"> ' . $failed . ' failed / ' . $logins . ' logins. ' . $cracked . ' cracked / ' . $attempts. '  attempts <br>';

    echo '</td></tr>';
    echo '</table>';

    $files = scandir('atomimages/');
    foreach($files as $filename) {

	if ('.' === $filename) continue;
	if ('..' === $filename) continue;

	$file = substr($filename, 0, 4);
	if(strstr($password, $file)) {
	    echo '<img id="' . $file . '" border="1" src="atomimages/' . $filename . '" width="100" height="100" >';
	}
    }

}

mysql_close();
?>
<br>
</center>
