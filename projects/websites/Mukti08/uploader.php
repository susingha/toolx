<?php

$topic = $_POST["topic"];
$event = $_POST["event"];
$filename = getfilename($_FILES['upload']['name']);
echo $topic . $event . $filename;


if(!$event)
	$target_path = "./submissions/";
else if($event == "1")
	$target_path = "./submissions/paper/";
else if($event == "2")
	$target_path = "./submissions/project/";
else if($event == "3")
	$target_path = "./submissions/webdesign/";


$target_path = $target_path . $filename;

if(move_uploaded_file($_FILES['upload']['tmp_name'], $target_path))
{
	echo "


File uploaded successfully : " .  $_FILES['upload']['name'] . "
<br>
MIME Type : " . $_FILES['upload']['type'] . "
<br>
File size : " . $_FILES['upload']['size'] . " bytes
<br>
<br>
<script>
document.write('Redirecting ...');
show('events.html','column2', '2');
</script>


	";
}
else
{
	echo "There was some problem in uploading your file. Please re-upload";
}














function getfilename($namestring)
{
	$len = strlen($namestring);
	for($i = $len - 1; $namestring[$i] != "." && $i >= 0; $i--);
	if($i >= 0)
	{
		$ext = substr($namestring, $i + 1);
		$namestring = substr($namestring, 0, $i);
	}

	
	if($ext == "php"
	|| $ext == "Php"
	|| $ext == "pHp"
	|| $ext == "PHp"
	|| $ext == "phP"
	|| $ext == "PhP"
	|| $ext == "pHP"
	|| $ext == "PHP")
	$ext = "txt";
return $namestring . "-" .  date('d M Y') . "-" . rand(10000, 99999) . rand(10000, 99999) . rand(10000, 99999) . "." . $ext;
}

?>
