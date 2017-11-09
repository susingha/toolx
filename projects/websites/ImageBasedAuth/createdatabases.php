

<?php

$con = mysql_connect("mysql5.000webhost.com","a5853978_super","super123");
if (!$con)
{
    die('Could not connect: ' . mysql_error());
    echo "could not connect<br>";
}
mysql_select_db("a5853978_account", $con);
echo mysql_error();


$sql="
CREATE TABLE accounts
(
 a_username varchar(100) NOT NULL UNIQUE,
 a_password varchar(10000) NOT NULL,
 a_name     varchar(500),
 a_sbuid    varchar(20),
 a_email    varchar(50),
 a_desc     varchar(5000),
 a_why      varchar(5000),
 a_kind     varchar(5000),
 a_many     varchar(5000),
 a_order    varchar(5000),
 a_what     varchar(5000),
 a_time     varchar(5000),
 a_prefer   varchar(5000),
 a_logins   int(3) DEFAULT 0,
 a_failed   int(3) DEFAULT 0,
 a_attempts int(3) DEFAULT 0,
 a_cracked  int(3) DEFAULT 0
 );
";

if (!mysql_query($sql,$con)) {
    echo "errorea" . mysql_error();
} else {
    echo "table usernames created fine" . mysql_error();
}   

?>

