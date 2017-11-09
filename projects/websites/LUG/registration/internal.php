<center>
<?php
if($_SERVER['REMOTE_ADDR'] == "127.0.0.1"
|| $_SERVER['REMOTE_ADDR'] == "210.212.5.146"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.135"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.139"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.140"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.146"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.149"
|| $_SERVER['REMOTE_ADDR'] == "220.225.80.150")
;
else
{
	echo 	'
	<br><br><h3 style="color: #AAAAAA">link to internal registration has been blocked because probably you are out side the NIT-DGP network</h3>
	<h3><a onclick=showpage("registration/external.php")>Register as user (outside NIT-Dgp network)</a></h3>
		';
	return;
}

?>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table2">
	<tr>
		<td>
		<h3><a href=".">Home</a> : Registration(membership)</h3>
		</td>
	</tr>
</table>

<h1>Register as a Member of LUG NIT Durgapur.</h1>


<form method="post" action="registration/register_internal.php" onsubmit="return validate('username','pass','from','linux')">
<table border="0" cellpadding="0" id="table1" cellspacing="7">
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Username :</td>
		<td><input type="text" name="username" id="username">* will appear across ur uploads</td>
	</tr>
	<tr>
		<td align="right"></td>
		<td><input type="button" value="Check Availability" id="check"  onClick="check_availability()"></td>
	</tr>
	<tr>
		<td align="right">Password :</td>
		<td><input type="password" name="password" id="pass">*</td>
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