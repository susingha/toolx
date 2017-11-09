<?php session_start();
$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT no FROM papers WHERE username = '" . $_GET["username"] . "'";
$result = mysql_query($query);
$papers = mysql_num_rows($result);


$query = "SELECT no FROM articles WHERE username = '" . $_GET["username"] . "'";
$result = mysql_query($query);
$articles = mysql_num_rows($result);


$query = "SELECT * FROM usernames WHERE username = '" . $_GET["username"] . "'";
$result = mysql_query($query);

while($row = mysql_fetch_array($result))
{
	$member = $row["member"];
	$name = $row["name"];
	$frm = $row["frm"];
	$email = $row["email"];
	$phone = $row["phone"];
	switch($row["linux"])
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

	$regdate = $row["regdate"];
}

if($member)
{
	$query = "SELECT * FROM internal WHERE username = '" . $_GET["username"] . "'";
	$result = mysql_query($query);

	while($row = mysql_fetch_array($result))
	{
		$dept = $row["dept"];
		$roll = $row["roll"];
		$yrjoin = $row["yrjoin"];
		$regno = $row["regno"];
		if($row["paid"])
			$paid = 'YES';
		else
			$paid = 'NO';
			
		$post = $row["post"];
		if($row["admin"])
			$admin = 'YES';
		else
			$admin = 'NO';
	}

}
else
{
	$query = "SELECT * FROM external WHERE username = '" . $_GET["username"] . "'";
	$result = mysql_query($query);

	while($row = mysql_fetch_array($result))
	{
		$dob = $row["dob"];
		$inst = $row["inst"];
		$spec = $row["spec"];
		$others = $row["others"];
	}

}		
?>
<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%">
	<tr>
		<td>
		<h3><a <?php if(!$loggedin) echo 'href="."'; else echo 'onclick=showpage("login/home.php")'; ?> >Home</a> : Profile</h3>		
		<br>
		</td>
	</tr>
	<tr>
		<td>	
		
		<h2>Profile</h2>
		<h1 style="margin-bottom: 0;"><?php echo $_GET["username"]; ?></h1>
		<?php echo $post ; ?>
		
		
		<h3 style="margin-top: 20;"><?php echo $_GET["username"]; ?> has contributed ...</h3>
		<?php echo $papers; ?> papers<br>
		<?php echo $articles; ?> articles<br>
		<br>
		
		</td>
	</tr>
	<tr>
		<td>
		
		
		<table border="0" style="border-collapse: collapse" width="100%" id="table1" cellspacing="5">
			<tr>
				<td align="left" valign="top" width="140">Name :</td>
				<td align="left" valign="top"><b><?php echo $name; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">From :</td>
				<td align="left" valign="top"><b><?php echo $frm; ?></b></td>
			</tr>
			<?php if($loggedin >= 3){ ?>
			<tr>
				<td align="left" valign="top" width="140">Email :</td>
				<td align="left" valign="top"><b><?php echo $email; ?></b></td>
			</tr>			
			<tr>
				<td align="left" valign="top" width="140">Phone :</td>
				<td align="left" valign="top"><b><?php echo $phone; ?></b></td>
			</tr>
			<?php } ?>
			<tr>
				<td align="left" valign="top" width="140">About Linux :</td>
				<td align="left" valign="top"><b><?php echo $linux; ?></b></td>
			</tr>			
			<tr>
				<td align="left" valign="top" width="140">Date&nbsp;of&nbsp;registration&nbsp;:</td>
				<td align="left" valign="top"><b><?php echo $regdate; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">&nbsp;</td>
				<td align="left" valign="top">&nbsp;</td>
			</tr>

<?php 
if($member){
?>
			<tr>
				<td align="left" valign="top" width="140">Department :</td>
				<td align="left" valign="top"><b><?php echo $dept; ?></b></td>
			</tr>
			<?php if($loggedin >= 3){ ?>
			<tr>
				<td align="left" valign="top" width="140">Roll :</td>
				<td align="left" valign="top"><b><?php echo $roll; ?></b></td>
			</tr>
			<?php } ?>
			<tr>
				<td align="left" valign="top" width="140">Year of join :</td>
				<td align="left" valign="top"><b><?php echo $yrjoin; ?></b></td>
			</tr>
			<?php if($loggedin >= 3){ ?>
			<tr>
				<td align="left" valign="top" width="140">Registration No :</td>
				<td align="left" valign="top"><b><?php echo $regno; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">Paid :</td>
				<td align="left" valign="top"><b><?php echo $paid; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">Admin :</td>
				<td align="left" valign="top"><b><?php echo $admin; ?></b></td>
			</tr>
			<?php } ?>
			<tr>
				<td align="left" valign="top" width="140">&nbsp;</td>
				<td align="left" valign="top">&nbsp;</td>
			</tr>
<?php
}else{
?>
			<tr>
				<td align="left" valign="top" width="140">Date of Birth :</td>
				<td align="left" valign="top"><b><?php echo $dob; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">Institute :</td>
				<td align="left" valign="top"><b><?php echo $inst; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">Specialization :</td>
				<td align="left" valign="top"><b><?php echo $spec; ?></b></td>
			</tr>
			<tr>
				<td align="left" valign="top" width="140">Others :</td>
				<td align="left" valign="top"><b><?php echo $others; ?></b></td>
			</tr>
<?php
}
?>
		</table>		
		
		</td>
	</tr>	
</table>
</center>