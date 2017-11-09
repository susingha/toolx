<?php
session_start();
$loggedin = 0;
session_unregister("loggedin");
session_destroy();
setcookie("user","",time()-10,"/");
setcookie("rcook","",time()-10,"/");
?>