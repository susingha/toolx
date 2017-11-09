<center>

Please login here :
<img border="0" src="images/here.png">
<br>
<br>
<h2>
<?php
echo "Welcome " . $username;
?>
<br>Thank you for registering</h2>
Linux Users' Group
<br>
NIT Durgapur
<br>
<br>
<br>
<b>You are NOT YET a MEMBER of<br>
LUG - NIT Durgapur<br><br></b>
<h3>Please report to the LUG ROOM or any of the LUG Committee Members<br>
with an amount of Rs. 50 (Fifty Rupees), your USERNAME and SECRET CODE<br></h3>
your SECRET CODE is : 
<b>
<?php
echo $rand;
session_destroy();
?>
</b>
<br><br><br>
<i>Produce your <b>Membership Card</b>, in case you are already a paid member of LUG-NITDGP.</i>
</center>