

<?php
$crack = $_GET["crack"];
$username = $_GET["username"];
if($crack == "") {
    $crack = 0;
}
?>

<center>
<br><br><br><br><br><br><br><br>
<form method="POST" action="choose.php?crack=<?php echo $crack; ?>">
Username: 
<input type="text" name="username" <?php if ($crack) echo 'readonly="readonly" value="' . $username . '"'; ?> required>
<input type="submit" value="Submit and choose your password">
</form>
</center>
