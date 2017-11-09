<?php
session_start();
if($loggedin && isset($_COOKIE["user"]) && isset($_COOKIE["rcook"]))
{
	echo $username;
}
else
{
	echo "Guest";
}

?>