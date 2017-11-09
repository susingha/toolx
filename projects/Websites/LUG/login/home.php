<?php
session_start();
if($loggedin == 0 || !isset($_COOKIE["user"]) || !isset($_COOKIE["rcook"]) || $_COOKIE["user"] != $username || $_COOKIE["rcook"] != $cookie)
{
	//redirect to index.html
	echo "<center><br><br>sorry you are not allowed to view this page, Please login.<center>";
	return;
}


if($username == $regno)
{
	include "./existing_members/change.php";
	session_unregister("regno");
	return;
}
?>

<?php

$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);



$query = "SELECT no FROM papers WHERE username = '" . $username . "'";
$result = mysql_query($query);
$papers = mysql_num_rows($result);


$query = "SELECT no FROM articles WHERE username = '" . $username . "'";
$result = mysql_query($query);
$articles = mysql_num_rows($result);




switch($loggedin)
{
case 1:
	$status = "User";
	break;
case 2:
	$status = "Member";
	break;
case 3:
	$status = "Admin";
}



$query = "SELECT * FROM usernames WHERE username = '" . $username . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	$name = $row['name'];
	$frm = $row['frm'];
	$email = $row['email'];
	$phone = $row['phone'];
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
		
	$regdate = $row['regdate'];
	break;	
}

?>


<center>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="90%" id="table1" cellspacing="2">
	<tr>
		<td>
		<br>
		
		<table border="0" cellpadding="0" width="100%" id="table2" cellspacing="0">
		<tr>
		<td align="left" valign="top">
		<h2>You are logged in as <?php echo $username; ?></h2>
		Status : <?php echo $status; ?> : <a onclick="document.getElementById('t3').value = 'exit'; commandentered(13);">logout</a>
		<br>
		<br>
		<h3>You have contributed ...</h3>
		<a onclick="showpage('login/showpapers.php');"><?php echo $papers; ?> papers</a><br>
		<a onclick="showpage('login/showarticles.php');"><?php echo $articles; ?> articles</a><br>
	<!--x posts-->
		</td>
		<td align="right" valign="top">
		<table border="0" cellpadding="0" width="100%" id="table6">
			<tr>
				<td align="right"><i>Name :</i></td>
				<td align="left">&nbsp;<?php echo $name; ?></td>
			</tr>
			<tr>
				<td align="right"><i>From :</i></td>
				<td align="left">&nbsp;<?php echo $frm; ?></td>
			</tr>
			<tr>
				<td align="right"><i>E-Mail : </i> </td>
				<td align="left">&nbsp;<?php echo $email; ?></td>
			</tr>
			<tr>
				<td align="right"><i>Phone :</i></td>
				<td align="left">&nbsp;<?php echo $phone; ?></td>
			</tr>
			<tr>
				<td align="right"><i>About Linux :</i></td>
				<td align="left">&nbsp;<?php echo $linux; ?></td>
			</tr>
			<tr>
				<td align="right"><i>Registered :</i></td>
				<td align="left">&nbsp;<?php echo $regdate; ?></td>
			</tr>
			<tr>
				<td align="right"><h3><a onclick="showpage('login/editprofile.php');">Change</a></h3></td>
				<td align="left"></td>
			</tr>
		</table>		
		</td>
		</tr>
		</table>
		
		<table border="0" cellpadding="0" width="100%" id="table2" cellspacing="0">
			<tr>
				<td align="left" valign="top" width="774">
				<h3><br>Upload a paper :</h3>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">
<form enctype="multipart/form-data" action="login/uploadpaper.php" method="POST" style="margin-top: 0; margin-bottom: 0">
<input type="text" name="topic" size="20" maxlength="50">&nbsp;<i>Specify a Topic</i><br>
<input type="text" name="catch" size="20" maxlength="250">&nbsp;<i>Specify a catch line</i><br>
<input type="hidden" name="MAX_FILE_SIZE0" value="10000000" />
<input name="uploadpaper" type="file" size="20" />
<select name="format" title="specify the file format of the file that you are uploading">
<option selected value="">specify file format</option>
<option value="pdf">pdf</option>
<option value="doc">doc</option>
<option value="ppt">ppt</option>
<option value="rtf">rtf</option>
<option value="html">html/htm</option>
<option value="txt">txt</option>
<option value="zip">zip</option>
<option value="rar">rar</option>
</select>
<input type="submit" value="Upload paper" /><br>
</form>				
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">				
				<h3><br>Upload an article :</h3>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">
<form enctype="multipart/form-data" action="login/uploadarticle.php" method="POST" style="margin-top: 0; margin-bottom: 0">
<input type="text" name="topic" size="20" id="articletopic" maxlength="50">&nbsp;<i>Specify a Topic</i><br>
<input type="text" name="catch" size="20" id="articlecatch" maxlength="250">&nbsp;<i>Specify a catch line</i><br>
<input type="hidden" name="MAX_FILE_SIZE1" value="5000000" />
<input name="uploadarticle" type="file" size="20" />
<select name="format" title="specify the file format of the file that you are uploading">
<option selected value="">specify file format</option>
<option value="txt">txt</option>
<option value="html">html/htm</option>
<option value="pdf">pdf</option>
<option value="doc">doc</option>
<option value="ppt">ppt</option>
<option value="rtf">rtf</option>
<option value="zip">zip</option>
<option value="rar">rar</option>
</select>
<input type="submit" value="Upload article" /><br>
</form>
				
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">				
				<h3><br>Write an article : <a onclick="toggletxtarea();" id="toggler">+ show textarea</a></h3>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">				
<div id="txtarea" class="hide">
<textarea cols="58" rows="30" id="content">
</textarea>
<br>
<input type="button" value="Submit article" onclick="uploadwrittenarticle();">
</div>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="774">&nbsp;</td>
			</tr>
		</table>
		
		</td>
	</tr>
</table>
<?php
if($loggedin <= 1)
{
	echo "</center>";
	mysql_close();
	return;
}
?>



<?php
$paid = "";
$showcode = "";
$query = "SELECT regno, post, paid FROM internal WHERE username = '" . $username . "'";
$result = mysql_query($query);
while($row = mysql_fetch_array($result))
{
	$regno = $row["regno"];
	$post = $row["post"];
	if(!$row["paid"])
	{
		$paid = "NO";
		$query1 = "SELECT secretcode FROM secretcodes WHERE username = '" . $username . "'";
		$result1 = mysql_query($query1);
		while($row1 = mysql_fetch_array($result1))
		{
			$showcode = " | secret code : " . $row1["secretcode"];
		}		
	}
	else
	{
		$paid = "YES";
	}
}


?>
<br>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="85%" id="table1" cellspacing="2">
	<tr>
		<td>
		<h1>Membership</h1>
		<?php if($post != "") echo $post . '<br>'; ?>
		Registration No : <?php echo $regno; ?><br>
		Paid : <?php echo $paid . $showcode; ?>		
		
<!--		<table border="0" cellpadding="0" width="100%" cellspacing="0">
		<tr>
			<td align="left" valign="top" width="774">
				<h3><br>Submit a news piece.</h3>
			</td>
		</tr>
		<tr>
			<td>
<form action="login/uploadnews.php" method="POST" style="margin-top: 0; margin-bottom: 0">
<input type="text" name="headline">&nbsp;<i>Specify Headline</i><br>
<input type="text" name="link">&nbsp;<i>Specify a link</i><br>
<input type="text" name="desc" size="58">&nbsp;<i>Brief Description</i><br>
<input type="submit" value="Submit">
</form>
			</td>
		</tr>
		</table>//-->
		</td>
	</tr>
</table>
<?php
if($loggedin <= 2)
{
	echo "</center>";
	mysql_close();
	return;
}
?>




<br>
<table border="0" cellpadding="0" style="border-collapse: collapse" width="85%" id="table1" cellspacing="2">
	<tr>
		<td>		
		<h1>Admin</h1>
		<a onclick="showpage('login/admin/moderate.php');">Moderate uploads</a>
		<br>
		<br>
		<h3>Grant membership</h3>
		<table border="0" cellpadding="0" style="border-collapse: collapse" id="table3">
			<tr>
				<td>Username</td>
				<td>Secret code</td>
				<td></td>
			</tr>
			<tr>
				<td><input type="text" id="usr1">&nbsp;&nbsp;</td>
				<td><input type="text" id="sec">&nbsp;&nbsp;</td>
				<td><input type="button" value="PAID" onclick="showpage('login/admin/paid.php?usr=' + document.getElementById('usr1').value + '&sec=' + document.getElementById('sec').value);"></td>
			</tr>
			<tr>
				<td>&nbsp;</td>
				<td>Registration No.</td>
				<td></td>
			</tr>
			<tr>
				<td><i>OR</i></td>
				<td><input type="text" id="regg">&nbsp;&nbsp;</td>
				<td><input type="button" value="PAID" onclick="showpage('login/admin/paid2.php?regno=' + document.getElementById('regg').value);"></td>
			</tr>
		</table>
		
		<br>
		<h3>Grant post</h3>
		<table border="0" cellpadding="0" style="border-collapse: collapse" id="table4">
			<tr>
				<td>Username</td>
				<td>Post</td>
				<td></td>
			</tr>
			<tr>
				<td><input type="text" id="usr2">&nbsp;&nbsp;</td>
				<td><input type="text" id="post">&nbsp;&nbsp;</td>
				<td><input type="button" value="Grant Post" onclick="showpage('login/admin/grantpost.php?usr=' + document.getElementById('usr2').value + '&post=' + document.getElementById('post').value);"></td>
			</tr>
		</table>
		
		<br>
		<h3>Grant administrator status</h3>
		<table border="0" cellpadding="0" style="border-collapse: collapse" id="table5">
			<tr>
				<td>Username</td>
				<td>Registration No</td>
				<td></td>
			</tr>
			<tr>
				<td><input type="text" id="usr3">&nbsp;&nbsp;</td>
				<td><input type="text" id="reg">&nbsp;&nbsp;</td>
				<td><input type="button" value="Grant Admin" onclick="showpage('login/admin/grantadmin.php?usr=' + document.getElementById('usr3').value + '&regn=' + document.getElementById('reg').value);"></td>
			</tr>
		</table>
		
		<br>
		<h3>Ban user</h3>
		<table border="0" cellpadding="0" style="border-collapse: collapse" id="table5">
			<tr>
				<td>Username</td>
				<td></td>				
			</tr>
			<tr>
				<td><input type="text" id="usr4">&nbsp;&nbsp;</td>
				<td><input type="button" value="Delete User" onclick="showpage('login/admin/banuser.php?usr=' + document.getElementById('usr4').value);"></td>
			</tr>
		</table>

		</td>
	</tr>
</table>
</center>


<?php
mysql_close();
?>