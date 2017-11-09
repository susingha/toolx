

<a href="fill_in_details.html">Back</a>
<table>


<?php
$con = mysql_connect("localhost","root","");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  echo "could not connect<br>";
  }
mysql_select_db("lug_nitdgp", $con);


$query = "SELECT * FROM internal ORDER BY admin DESC";
$result = mysql_query($query);

while($row = mysql_fetch_array($result))
{
//	echo '<tr><td>'. $row["username"] . '</td><td>'. $row["password"] . '</td><td><a href=delete_this.php?regno='. $row["username"] .'>Delete</a></td></tr>';
echo '<tr><td>'. $row["username"] . '</td><td>'. $row["dept"] . '</td><td>'. $row["yrjoin"] . '</td><td>'. $row["paid"] . '</td><td>'. $row["admin"] . '</td></tr>';
}

?>


</table>

<a href="fill_in_details.html">Back</a>