<center>
<br>
<br>
<br>

File uploaded successfully : <?php echo $_GET["file"]; ?>
<br>
MIME Type : <?php echo $_GET["type"]; ?>
<br>
File size : <?php echo $_GET["size"]; ?> bytes
<br>
<br>
<br>
<h2>Your upload is waiting to be Moderated.</h2>

<script>
document.write('Redirecting ...');
showpage('login/home.php');
</script>

<noscript>
<a href="?page=login/home.php">back to home.</a>
</noscript>

</center>