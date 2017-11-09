<?php

session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}


if($loggedin <= 1)
{
	echo '<br>Sorry for member login only';
	return;
}


$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT password FROM usernames WHERE username = '" . $username . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	$password = $row["password"];
}


$query = "SELECT regno FROM internal WHERE username = '" . $username . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	if($username != $row['regno'])
	{
		echo "<br><br>Sorry you have already changed your username and password.";
		return;
	}
}


?>


<center>
<br>
<h2>Welcome member.<br>
Please change your username and password</h2>
<br>
You will not be asked to do this again, once you choose a different username.<br>
<i>You can change the rest of your account details once you login.</i>

<br>

<form method="post" action="login/existing_members/update.php" onsubmit="return validate('username','pass','from','linux')">
<table border="0" cellpadding="0" id="table1" cellspacing="7">
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Username :</td>
		<td><input type="text" name="username" id="username" value="<?php echo $username; ?>">* will appear across ur uploads</td>
	</tr>
	<tr>
		<td align="right"></td>
		<td><input type="button" value="Check Availability" id="check"  onClick="check_availability()"></td>
	</tr>
	<tr>
		<td align="right">Password :</td>
		<td><input type="password" name="password" id="pass" value="<?php echo $password; ?>">*</td>
	</tr>
	<tr>
		<td align="right">Re-type Password :</td>
		<td><input type="password" name="retype" id="repass"></td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
		<tr>
		<td align="right"><h3>Personal Information</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Name :</td>
		<td><input type="text" name="name"></td>
	</tr>
	<tr>
		<td align="right"><i>(place)</i> From :</td>
		<td><input type="text" name="from" id="from">*</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right"><h3>Contact Details</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">E-mail ID :</td>
		<td><input type="text" name="email"></td>
	</tr>
	<tr>
		<td align="right">Phone no :</td>
		<td><input type="text" name="phone"></td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right"><h3>Professional Information</h3></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Department :</td>
		<td><input type="text" name="dept"></td>
	</tr>
	<tr>
		<td align="right">Roll No :</td>
		<td><input type="text" name="roll"></td>
	</tr>
	<tr>
		<td align="right">Year of joining this institute:</td>
		<td>
		<select name="yrjoin">
		<option>2007</option>
		<option>2006</option>
		<option>2005</option>
		<option>2004</option>
		<option>2003</option>
		</select>
		</td>
	</tr>
	<tr>
		<td align="right">About Linux</td>
		<td>
		<select name="linux" size="1" id="linux">
			<option selected value="">Choose</option>
			<option value="10">I devolop Linux</option>
			<option value="9">I am a Linux GURU</option>
			<option value="8">I am a Linux Professional</option>
			<option value="7">I am always on Linux</option>
			<option value="6">I love Linux</option>
			<option value="5">I have worked on Linux</option>
			<option value="4">I know a little Linux</option>
			<option value="3">I have just started Linux</option>
			<option value="2">Ya, I have seen Linux</option>
			<option value="1">I have heard abour Linux</option>
			<option value="0">Linux!! Whos that!!</option>							
		</select>
		*</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">&nbsp;</td>
		<td><input type="submit" value="Register Me"></td>
	</tr>
	
</table>
</form>



</center>