
<script>

var passString = "-";
function concatPassString(substring) {
    var tmp = passString.concat(substring, "-");
    passString = tmp;
    document.getElementById("password").value = passString;

    var firstRow=document.getElementById("display").rows[0];
    var x = firstRow.insertCell(-1);

    var file = "atomimages/" + substring + ".jpg";
    x.innerHTML='<img src=' + file + ' width="100" height="100">';
}

function showPassString() {
    alert(passString);
}


function resetPassString() {
    passString = "-";
    var firstRow = document.getElementById("display").rows[0];
    while (firstRow.firstChild) {
	    firstRow.removeChild(firstRow.firstChild);
    }
}
function pressImage(id) {
    document.getElementById(id).height = 90;
}

function releaseImage(id) {
    document.getElementById(id).height = 100;
}

</script>

<!--
<input type="button" value="Show  Pass String" id="show"  onclick="showPassString()">
<br><hr>
-->

<form method="POST" action="register.php">
<table><tr><td width="65%" style="vertical-align:top;">
<table border="1" cellpadding="5" align="center" style="border-collapse: collapse;" width="100%">
<tr><td>
Username: 
<input type="text" name="username" required>
Name:
<input type="text" name="name">
<input type="hidden" name="password" id="password" value="-"><br>
Please click through images below to choose your password:
<div style="background-image: url(images/back.jpeg); height: 100px; width: 100px;"><table id="display"><tr></tr></table></div>
<br>
<i>submit button at end of the page</i>
<input type="button" value="Reset Pass String" id="reset" onclick="resetPassString()">

</td></tr>

<tr><td>
Faces
<div style="position: relative; width: 800px; overflow: auto">
<table border="0" width="100%"><tr>
<?php
$start = 1000;
$end = $start + 1000;
for($i = $end - 1; $i > $start; $i--) {
    $imagename = 'atomimages/' . $i . '.jpg';
    if (file_exists($imagename)) {
	echo '<td>';
	echo '<img id="' . $i . '" border="0" src="' . $imagename . '" width="100" height="100" onclick=concatPassString("' . $i . '") onmousedown=pressImage(' . $i . ') onmouseup=releaseImage(' . $i . ') onmouseout=releaseImage(' . $i . ')>';
	echo '</td>';
    }
}
?>
</tr></table>
</div>
</td></tr>

<tr><td>Chairs
<div style="position: relative; width: 800px; overflow: auto">
<table border="0" width="100%"><tr>
<?php
$start = 2000;
$end = $start + 1000;
for($i = $start + 1; $i < $end; $i++) {
    $imagename = 'atomimages/' . $i . '.jpg';
    if (file_exists($imagename)) {
	echo '<td>';
	echo '<img id="' . $i . '" border="0" src="' . $imagename . '" width="100" height="100" onclick=concatPassString("' . $i . '") onmousedown=pressImage(' . $i . ') onmouseup=releaseImage(' . $i . ') onmouseout=releaseImage(' . $i . ')>';
	echo '</td>';
    }
}
?>
</tr></table>
</div>
</td></tr>

<tr><td>Dogs
<div style="position: relative; width: 800px; overflow: auto">
<table border="0" width="100%"><tr>
<?php
$start = 3000;
$end = $start + 1000;
for($i = $start + 1; $i < $end; $i++) {
    $imagename = 'atomimages/' . $i . '.jpg';
    if (file_exists($imagename)) {
	echo '<td>';
	echo '<img id="' . $i . '" border="0" src="' . $imagename . '" width="100" height="100" onclick=concatPassString("' . $i . '") onmousedown=pressImage(' . $i . ') onmouseup=releaseImage(' . $i . ') onmouseout=releaseImage(' . $i . ')>';
	echo '</td>';
    }
}
?>
</tr></table>
</div>
</td></tr>

<tr><td>Landscapes
<div style="position: relative; width: 800px; overflow: auto">
<table border="0" width="100%"><tr>
<?php
$start = 4000;
$end = $start + 1000;
for($i = $start + 1; $i < $end; $i++) {
    $imagename = 'atomimages/' . $i . '.jpg';
    if (file_exists($imagename)) {
	echo '<td>';
	echo '<img id="' . $i . '" border="0" src="' . $imagename . '" width="100" height="100" onclick=concatPassString("' . $i . '") onmousedown=pressImage(' . $i . ') onmouseup=releaseImage(' . $i . ') onmouseout=releaseImage(' . $i . ')>';
	echo '</td>';
    }
}
?>
</tr></table>
</div>
</td></tr>

<tr><td>Abstract
<div style="position: relative; width: 800px; overflow: auto">
<table border="0" width="100%"><tr>
<?php
$start = 5000;
$end = $start + 1000;
for($i = $start + 1; $i < $end; $i++) {
    $imagename = 'atomimages/' . $i . '.jpg';
    if (file_exists($imagename)) {
	echo '<td>';
	echo '<img id="' . $i . '" border="0" src="' . $imagename . '" width="100" height="100" onclick=concatPassString("' . $i . '") onmousedown=pressImage(' . $i . ') onmouseup=releaseImage(' . $i . ') onmouseout=releaseImage(' . $i . ')>';
	echo '</td>';
    }
}
?>
</tr></table>
</div>
</td></tr>

</table>
</td>
<td width="35%" style="vertical-align:top;">
Please answer a few questions:
<br>

SBU ID: &nbsp; <input type="text" name="sbuid" placeholder="If you are an SBU Student">
<br>
Email ID: <input type="email" name="email" placeholder="we would contact you sporadically">
<br><br>

Describe you password <br>
<textarea name="desc" rows="10" cols="45" placeholder="Describe as much as you can, so that someone else can guess it." required>
</textarea>
<br><br>

Why did you choose this password <br>
<textarea name="why" rows="4" cols="45" placeholder="May be you thought this is the combination you woule be able to remember best, or is the most difficult to express / guess by an attacker">
</textarea>
<br><br>

Do you prefer images of the same kind or images of different kinds<br>(from multiple rows in this case) <br>
<textarea name="kind" rows="4" cols="45" placeholder="You may also mention any other category that you would prefer">
</textarea>
<br><br>

How many images do you think you can remember and you would use as a password in reality <br>
<textarea name="many" rows="4" cols="45" placeholder="Provided you are forced to use this kind of passwords i.e. You may say you would like 2 faces and 3 landscapes to remember best">
</textarea>
<br><br>

Would you like to remember your images in the same order you chose or as a set of images to choose from in any order.
<textarea name="order" rows="4" cols="45" placeholder="Give a subjective answer. You will be prompted with a small set of images when you try to login with your image based password">
</textarea>
<br><br>

What were you thinking that made you click the images that you clicked<br>
<textarea name="what" rows="4" cols="45" placeholder="You may give a vague answer for this">
</textarea>
<br><br>

How long do you think you would remember your password.<br>
<textarea name="time" rows="4" cols="45" placeholder="days, weeks, months, years.. Also mention provided if at what frequency you have been using these passwords">
</textarea>
<br><br>

Would you prefer this kind of a password for your personal use or business use or both ? <br>
<textarea name="prefer" rows="4" cols="45" placeholder="You may also rant about why you completely oppose this idea">
</textarea>
<br><br>



</td>
</tr></table>
<input type="submit" value="Submit">
</form> 
