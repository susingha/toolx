<?php
session_start();
?>
<html>
<head>
<title>Linux Users' Group, NIT Durgapur</title>
<link rel="stylesheet" type="text/css" href="style.css"></link>
<link rel='SHORTCUT ICON' href='./images/paw.gif'>
<script type="text/javascript" src="ajaxscripts.js"></script>
</head>





<body topmargin="0" leftmargin="0" rightmargin="0" bottommargin="0" marginwidth="0" marginheight="0" background="images/back.PNG">


<table name="table1" border="0" cellpadding="0" style="border-collapse: collapse" width="100%" height="100%">
	<tr>
		<td align="center" valign="top" width="129" background="images/back2.PNG">		
		
		
		<a href="http://www.lug.nitdgp.ac.in"><img border="0" src="images/logo.png"></a>
		<table border="0" cellpadding="0" style="border-collapse: collapse" width="100%">
		<tr><td align="center" height="22">
		<div id="loading" class="hide"><img src="images/loading.gif"></div>
		</td></tr>
		<tr><td align="center" valign="top">
		<font size="2">
			<b>			
<div id="logname">
<?php
if($loggedin && isset($_COOKIE["user"]) && isset($_COOKIE["rcook"]))
{
	echo "Welcome " . $_COOKIE["user"];
}
else
{
	echo "Welcome Guest";
}
?>
</div>
			</b>
		</font>
		</td></tr>
		</table>
		
		<div class="sidemenu1">
			<h3>News</h3>
			<a href="?page=noticeboard.php">Notice Board</a>			
			<!--<a href=#>Linux News</a>-->
			<!--<a href=#>Coming Up</a>-->
			<h3>Downloads</h3>
			<a href="./download/linux" target="_blank">Linux</a>
			<a href="./download/softwares" target="_blank">Softwares</a>
			<h3>Archives</h3>
			<a href="?page=showpapers.php&count=5">Recent Papers</a>
			<a href="?page=showarticles.php&count=5">Recent Articles</a>
			<a href="?page=showpapers.php">All Papers</a>			
			<a href="?page=showarticles.php">All Articles</a>
		<!--<h3>Forum</h3>
			<a href=#>Recent topics</a>
			<a href=#>All topics</a>
			<a href=#>Start new topic</a>-->
		</div>
		<br>
		<h3>
		<a onclick="showpage('login/home.php');">My Home</a>
		</h3>
		
		</td>		
		<td bgcolor="#820000">|
		</td>
		<td>
<table name="table2" border="0" cellpadding="0" style="border-collapse: collapse" width="100%" height="100%" id="table1">
	<tr>
		<td height="65" bgcolor="#3E0000" valign="top">
		<table name="table3" border="0" cellpadding="0" style="border-collapse: collapse" width="100%" id="table2" height="100%">
			<tr>
				<td align="left" width="260">
				<a href="http://www.lug.nitdgp.ac.in"><img border="0" src="images/banner.PNG"></a>
				</td>
				<td bgcolor="#FFFFFF" width="2"></td>
				<td bgcolor="#000000" width="5"></td>
				<td align="left">
				
				<form name="form1" onsubmit="return stopsubmit()" style="margin-top: 0; margin-bottom: 0">
				<table name="table4" border=0 cellspacing="0" cellpadding="0" width="97%" height="100%">
<?php
if(!$loggedin){
echo '				
						<tr>
						<td align="left" valign="top">
						<div id="l1"><font color="#B7B7B7" size="2">Linux&nbsp;Login&nbsp;-&nbsp;Username&nbsp;:</font><input type="text" name="text1" class="hiddentext" size="10" id="t1"  onkeypress="usernameentered(event.which)"></div>
						</td>
						</tr>
						
						<tr>
						<td align="left">
						<div id="l2">&nbsp;</div>
						</td>
						</tr>
						
						<tr>
						<td align="left">
						<div id="l3">&nbsp;</div>
						</td>
						</tr>							
';}
else
{
echo '
						<tr>
						<td align="left" valign="top">
						<div id="l1"><font color="#B7B7B7" size="2">linux@localhost&nbsp;#</font><input type="text"  name="text4" class="hiddentext" size="25" id="t4" onkeypress="commandentered(event.which)"></div>
						</td>
						</tr>
						
						<tr>
						<td align="left">
						<div id="l2"><font color="#B7B7B7" size="2">linux@localhost&nbsp;#</font><input type="text"  name="text4" class="hiddentext" size="25" id="t4" onkeypress="commandentered(event.which)"></div>
						</td>
						</tr>
						
						<tr>
						<td align="left">
						<div id="l3"><font color="#B7B7B7" size="2">linux@localhost&nbsp;#</font><input type="text"  name="text3" class="hiddentext" size="25" id="t3" onkeypress="commandentered(event.which)"></div>
						</td>
						</tr>
';
}
?>	


				</table>
				</form>			
				</td>
			</tr>
		</table>

		</td>
		<td width="20%" height="65" bgcolor="#3E0000" align="right" valign="top">
<form action="http://www.google.com/search" method="get" style="margin-top: 0; margin-bottom: 0" onSubmit="Gsitesearch(this);" target="_blank">		
<input value="Search" id="query" name="qfront" type="text" class="searchbox" onblur="if(this.value=='')this.value='Search';" onfocus="if(this.value=='Search')this.value='';"><br>
<input name="q" type="hidden">
<select id="site" name="domainroot" class="searchbox">
<option value="lug.nitdgp.ac.in" selected="1">LUG NITDgp</option>
<option value="www.nitdgp.ac.in">NIT Durgapur</option>
</select><br>
<input type="submit" value="G Search" class="searchbox">
</form>
		</td>
	</tr>
	<tr>
		<td bgcolor="#FFFFFF">
		
		<table border="0" cellpadding="0" style="border-collapse: collapse" width="100%" height="100%" id="mainmenu">
			<tr class="mainmenu">
				<td align="right" valign="top"><img border="0" src="images/leftcorner.PNG"></td>
				
				<td align="center" background="images/backcolor.PNG"><a href=".">Home</a></td>
				<td align="center" background="images/backcolor.PNG"><a href="?page=aboutus.html">About&nbsp;Us</a></td>
				<td align="center" background="images/backcolor.PNG"><a href="?page=whoswho.html">Whos&nbsp;Who</a></td>
				<td align="center" background="images/backcolor.PNG"><a href=#>Gallery</a></td>
				<td align="center" background="images/backcolor.PNG"><a href="?page=contactus.html">Contact&nbsp;Us</a></td>
				
				<td align="left" valign="top"><img border="0" src="images/rightcorner.PNG"></td>
			</tr>
		</table>
		
		</td>
		<td height="29">&nbsp;</td>
	</tr>
	<tr>
		<td bgcolor="#FFFFFF" valign="top">
		<div id = "contents">	




		<?php
		
if($_GET["page"]=="")	
{
	if($loggedin && isset($_COOKIE["user"]) && isset($_COOKIE["rcook"]))
		include "login/home.php";
	else
		include "./home.php";
}
else
{
	$page = $_GET["page"];
	include $page;

/*
commented
$f=fopen($page,"r") or exit("Unable to open file!");	while (!feof($f))	{		$x=fgetc($f);		echo $x;	}	fclose($f);
*/

}
		?>		
		</div>
<br>
<center>

<script type="text/javascript"><!--
google_ad_client = "pub-1635181638107169";
google_ad_width = 468;
google_ad_height = 60;
google_ad_format = "468x60_as";
google_ad_type = "text";
google_ad_channel = "";
google_ui_features = "rc:6";
//-->
</script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>

</center>		
		</td>
		<td>
		</td>
	</tr>
	<tr id="footer" height="26">
		<td bgcolor="#3E0000" align="center">©Copyright 2007 Linux Users' Group, NIT Durgapur</td>
		<td align="left"><img border="0" src="images/edge.gif" width="27" height="100%"></td>
	</tr>
</table>
		</td>
		</tr>
</table>
</body>
</html>