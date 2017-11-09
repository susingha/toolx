<?php
session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}


$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT * FROM usernames WHERE username = '" . $username . "'";
$result = mysql_query($query);

while($row = mysql_fetch_array($result))
{
	$password = $row['password'];
	$member = $row['member'];
	
	$name = $row['name'];
	$frm = $row['frm'];

	$email = $row['email'];
	$phone = $row['phone'];
	
	$index = $row['linux'];
	switch($row['linux'])
	{
	case 0:
		$linux = "Linux!! Whos that!!";
		break;
	case 1:
		$linux = "I have heard abour Linux";
		break;
	case 2:
		$linux = "Ya, I have seen Linux";
		break;
	case 3:
		$linux = "I have just started Linux";
		break;
	case 4:
		$linux = "I know a little Linux";
		break;
	case 5:
		$linux = "I have worked on Linux";
		break;
	case 6:
		$linux = "I love Linux";
		break;
	case 7:
		$linux = "I am always on Linux";
		break;
	case 8:
		$linux = "I am a Linux Professional";
		break;
	case 9:
		$linux = "I am a Linux GURU";
		break;
	case 10:
		$linux = "I devolop Linux";
	}
}


if($member)
{
	$query = "SELECT * FROM internal WHERE username = '" . $username . "'";
	$result = mysql_query($query);
	while($row = mysql_fetch_array($result))
	{
		$dept = $row['dept'];
		$roll = $row['roll'];
		$yrjoin = $row['yrjoin'];
	}
	
?>
<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table2">
	<tr>
		<td align="left">
		<h3><a onclick="showpage('login/home.php');">My Home</a> : Edit Profile</h3>
		</td>
	</tr>
</table>


<form method="post" action="login/update_internal.php" onsubmit="return validate('username','pass','from','linux')">
<table border="0" cellpadding="0" id="table1" cellspacing="7">
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Username :</td>
		<td><input type="text" name="username" id="username" value="<?php echo $username; ?>" disabled>* will appear across ur uploads</td>
	</tr>
	<tr>
		<td align="right">Password :</td>
		<td><input type="password" name="password" id="pass" value="<?php echo $password; ?>">*</td>
	</tr>
	<tr>
		<td align="right">Re-type Password :</td>
		<td><input type="password" name="retype" id="repass" value="<?php echo $password; ?>"></td>
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
		<td><input type="text" name="name" value="<?php echo $name; ?>"></td>
	</tr>
	<tr>
		<td align="right"><i>(place)</i> From :</td>
		<td><input type="text" name="from" id="from" value="<?php echo $frm;?>">*</td>
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
		<td><input type="text" name="email" value="<?php echo $email; ?>"></td>
	</tr>
	<tr>
		<td align="right">Phone no :</td>
		<td><input type="text" name="phone" value="<?php echo $phone; ?>"></td>
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
		<td><input type="text" name="dept" value="<?php echo $dept; ?>"></td>
	</tr>
	<tr>
		<td align="right">Roll No :</td>
		<td><input type="text" name="roll" value="<?php echo $roll; ?>"></td>
	</tr>
	<tr>
		<td align="right">Year of joining this institute:</td>
		<td>
		<select name="yrjoin">
		<option><?php echo $yrjoin; ?></option>
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
			<option value="<?php echo $index; ?>" selected><?php echo $linux; ?></option>
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
		<td><input type="submit" value="Update Profile"></td>
	</tr>
	
</table>
</form>
</center>
<?php	
	
	
}
else
{

	$query = "SELECT * FROM external WHERE username = '" . $username . "'";
	$result = mysql_query($query);
	while($row = mysql_fetch_array($result))
	{
		$dob = $row['dob'];
		$inst = $row['inst'];
		$spec = $row['spec'];
		$others = $row['others'];
	}
	
?>
<center>
<form method="post" action="login/update_external.php" onsubmit="return validate('username','pass','from','linux')">
<table border="0" cellpadding="0" id="table1" cellspacing="7">
	<tr>
		<td align="right">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td align="right">Username :</td>
		<td><input type="text" name="username" id="username" value="<?php echo $username; ?>" disabled>* will appear across ur uploads</td>
	</tr>
	<tr>
		<td align="right">Password :</td>
		<td><input type="password" name="password" id="pass" value="<?php echo $password; ?>">*</td>
	</tr>
	<tr>
		<td align="right">Re-type Password :</td>
		<td><input type="password" name="retype" id="repass" value="<?php echo $password; ?>"></td>
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
		<td><input type="text" name="name" value="<?php echo $name; ?>"></td>
	</tr>
	<tr>
		<td align="right"><i>(place)</i> From :</td>
		<td><input type="text" name="from" id="from" value="<?php echo $frm; ?>">*</td>
	</tr>
	<tr>
		<td align="right">Date of Birth :</td>
		<td>
		<select name="dd" size="1">
			<option selected><?php echo substr($dob,0,2); ?></option>
			<option>01</option>
			<option>02</option>
			<option>03</option>
			<option>04</option>
			<option>05</option>
			<option>06</option>
			<option>07</option>
			<option>08</option>
			<option>09</option>
			<option>10</option>
			<option>11</option>
			<option>12</option>
			<option>13</option>
			<option>14</option>
			<option>15</option>
			<option>16</option>
			<option>17</option>
			<option>18</option>
			<option>19</option>
			<option>20</option>
			<option>21</option>
			<option>22</option>
			<option>23</option>
			<option>24</option>
			<option>25</option>
			<option>26</option>
			<option>27</option>
			<option>28</option>
			<option>29</option>
			<option>30</option>
			<option>31</option>			
			</select>
			<select name="mm" size="1">
			<option selected><?php echo substr($dob,3,strlen($dob)-8); ?></option>
			<option>January</option>
			<option>February</option>
			<option>March</option>
			<option>April</option>
			<option>May</option>
			<option>June</option>
			<option>July</option>
			<option>August</option>
			<option>September</option>
			<option>October</option>
			<option>November</option>
			<option>December</option>						
			</select>
			<select name="yyyy" size="1">
			<option selected><?php echo substr($dob,strlen($dob)-4); ?></option>
			<?php
			for($i=2000;$i>1946;$i--)
				echo '<option>' . $i . '</option>';
			?>			
			</select>
		
		</td>
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
		<td><input type="text" name="email" value="<?php echo $email; ?>"></td>
	</tr>
	<tr>
		<td align="right">Phone no :</td>
		<td><input type="text" name="phone" value="<?php echo $phone; ?>"></td>
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
		<td align="right">Institute :</td>
		<td><input type="text" name="inst" value="<?php echo $inst; ?>"></td>
	</tr>
	<tr>
		<td align="right">Specialization :</td>
		<td><input type="text" name="spec" value="<?php echo $spec; ?>"></td>
	</tr>
	<tr>
		<td align="right">Others (occupation / year, etc etc) :</td>
		<td><input type="text" name="others" value="<?php echo $others; ?>"></td>
	</tr>
	<tr>
		<td align="right">About Linux :</td>
		<td>
		<select name="linux" size="1" id="linux">
			<option value="<?php echo $index; ?>" selected><?php echo $linux; ?></option>
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
		<td><input type="submit" value="Update Profile"></td>
	</tr>
	
</table>
</form>
</center>
<?php
}
?>