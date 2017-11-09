<html>
<body>



<?php
$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
 else
  {echo "MySQL connected" . mysql_error() . "<br>";

  }


echo "<br>";

$sql="CREATE DATABASE lug_nitdgp";
if (mysql_query($sql,$con))
  {
  echo "Database lug_nitdgp created" . mysql_error() . "<br>";
  }
else
  {
  echo "Error creating database: " . mysql_error() . "<br>";
  }





echo "<br>";

mysql_select_db("lug_nitdgp", $con);
echo mysql_error();




echo "<br>";

$sql="CREATE TABLE usernames
(
username varchar(30) NOT NULL UNIQUE,
password varchar(20) NOT NULL,
member int(1),

name varchar(30),
frm varchar(50),

email varchar(30),
phone varchar(15),

linux int(1),

regdate varchar(20),

PRIMARY KEY(username)
)";

if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table usernames created fine" . mysql_error();
	}









echo "<br>";

$sql="CREATE TABLE external
(
username varchar(30) NOT NULL UNIQUE,
dob varchar(20),

inst varchar(50),
spec varchar(50),
others varchar(50),

PRIMARY KEY(username)
)";

if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table external created fine" . mysql_error();
	}






echo "<br>";

$sql="CREATE TABLE internal
(
username varchar(30) NOT NULL UNIQUE,

dept varchar(30),
roll varchar(10),
yrjoin varchar(4),

regno varchar(8) NOT NULL UNIQUE,
paid int(1),
post varchar(20),
admin int(1),

PRIMARY KEY(username)
)";

if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table internal created fine" . mysql_error();
	}





echo "<br>";

$sql="CREATE TABLE secretcodes
(
username varchar(30) NOT NULL,
secretcode varchar(10) NOT NULL UNIQUE,
matter varchar(10)
)";
if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table secretcodes created fine" . mysql_error();}







echo "<br>";

$sql="CREATE TABLE papers
(
no varchar(5) NOT NULL UNIQUE,
moderated int(1) NOT NULL,
topic varchar(50),
catch varchar(250),
username varchar(30),
date varchar(20),
filename varchar(50) NOT NULL,
format varchar(5),
rating int(1),

PRIMARY KEY(no)
)";
if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table papers created fine" . mysql_error();}





echo "<br>";

$sql="CREATE TABLE articles
(
no varchar(5) NOT NULL UNIQUE,
moderated int(1) NOT NULL,
topic varchar(50),
catch varchar(250),
username varchar(30),
date varchar(20),
filename varchar(50) NOT NULL,
format varchar(5),
rating varchar(1),

PRIMARY KEY(no)
)";
if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table articles created fine" . mysql_error();}




echo "<br>";

$sql="CREATE TABLE news
(
moderated int(1) NOT NULL,

headline varchar(50),
descr varchar(250),
link varchar(250),

username varchar(30),
date varchar(20)
)";
if (!mysql_query($sql,$con))
	{echo "errorea" . mysql_error();}
else
	{echo "table news created fine" . mysql_error();}








//all tables created

mysql_close();
?>


</body>
</html>
