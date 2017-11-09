
<script>
var passString = "-";
function concatPassString(substring) {
    var tmp = passString.concat(substring, "-");
    passString = tmp;
    document.getElementById("password").value = passString;

    var firstRow=document.getElementById("display").rows[0];
    var x = firstRow.insertCell(-1);

    var file = "atomimages/" + substring + ".jpg";
    x.innerHTML='<img src=' + file + ' width="100" height="100">';
}

function pressImage(id) {
    document.getElementById(id).height = 90;
}

function releaseImage(id) {
    document.getElementById(id).height = 100;
}

function showPassString() {
    alert(passString);
}

function resetPassString() {
    passString = "-";
    var firstRow = document.getElementById("display").rows[0];
    while (firstRow.firstChild) {
	    firstRow.removeChild(firstRow.firstChild);
    }
}

</script>

<!--
<input type="button" value="Show  Pass String" id="show"  onclick="showPassString()">
<br><hr>
-->

<center>
<br><br>
<?php

$username = $_POST["username"];
$crack = $_GET["crack"];

if($crack)
    echo "Crack password of " . $username . ". Read the password hints below <br><br>";
else
    echo "Welcome " . $username . ". Choose your password <br><br>";

$con = mysql_connect("mysql5.000webhost.com","a5853978_super","super123");
if (!$con)
{
    die('Could not connect: ' . mysql_error());
    echo "could not connect<br>";
}
mysql_select_db("a5853978_account", $con);
echo mysql_error();

$query = "SELECT * from accounts WHERE a_username = '" . $username . "'";
$result = mysql_query($query);

while($row = mysql_fetch_array($result))
{
    $password = $row['a_password'];
    $name = $row[a_name];
    $desc = $row[a_desc];
    $why = $row[a_why];
    $kind = $row[a_kind];
    $many = $row[a_many];
    $order = $row[a_order];
    $what = $row[a_what];
    $time = $row[a_time];
    $prefer = $row[a_prefer];
}



$files = scandir('atomimages/');
foreach($files as $filename) {

    if ('.' === $filename) continue;
    if ('..' === $filename) continue;

    $rand = rand(0,20);
    $file = substr($filename, 0, 4);
    if(strstr($password, $file) || $rand == 0) {
	echo '<img id="' . $file . '" border="1" src="atomimages/' . $filename . '" width="100" height="100" onclick=concatPassString("' . $file . '") onmousedown=pressImage(' . $file . ') onmouseup=releaseImage(' . $file . ') onmouseout=releaseImage(' . $file . ')>';
    }
}


?>

<br><br>
<table width="40%"><tr><td>
<div style="background-image: url(images/back.jpeg); height: 100px; width: 100px;"><table id="display"><tr></tr></table></div>
</td></tr></table>
<br>


<form method="POST" action="verify.php?crack=<?php echo $crack; ?>">
<input type="hidden" name="username" value="<?php echo $username; ?>">
<input type="hidden" name="password" value="-" id="password">
<input type="button" value="Reset Pass String" id="reset" onclick="resetPassString()">
<input type="submit" value="Submit">
</form>

<div <?php if(!$crack) echo 'style="display: none;"'?>>
<br>
<hr>
<br>
Password Hints:
<br>

<table border="1" cellpadding="10" style="border-collapse: collapse;" width="70%"><tr><td style="vertical-align:left;"><tr><td>

<i>Describe you password</i> <br>
<?php echo $desc; ?>
<br><br><br>

<i>Why did you choose this password</i> <br>
<?php echo $why; ?>
<br><br><br>

<i>Do you prefer images of the same kind or images of different kinds (from multiple rows in this case)</i> <br>
<?php echo $kind; ?>
<br><br><br>

<i>How many images do you think you can remember and you would use as a password in reality</i> <br>
<?php echo $many; ?>
<br><br><br>

<i>Would you like to remember your images in the same order you chose or as a set of images to choose from in any order</i> <br>
<?php echo $order; ?>
<br><br><br>

<i>What were you thinking that made you click the images that you clicked</i> <br>
<?php echo $what; ?>
<br><br><br>

<i>How long do you think you would remember your password</i> <br>
<?php echo $time; ?>
<br><br><br>

<i>Would you prefer this kind of a password for your personal use or business use or both ?</i> <br>
<?php echo $prefer;

mysql_close();
?>
</td></tr></table>
</div>


</center>





