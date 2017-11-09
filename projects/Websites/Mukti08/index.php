<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">

<head>
  <title>LUG, NIT Durgapur presents MUKTI'08</title>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
  

  <!-- **** layout stylesheet **** -->
  <link rel="stylesheet" type="text/css" href="style/style.css" />

  <!-- **** colour scheme stylesheet **** -->
<?php
$rnd = rand(1,4);
switch($rnd)
{
case 1:
	echo ' <link rel="stylesheet" type="text/css" href="style/green.css" />';
	break;
case 2:
	echo ' <link rel="stylesheet" type="text/css" href="style/purple.css" />';
	break;
case 3:
	echo ' <link rel="stylesheet" type="text/css" href="style/blue.css" />';
	break;
case 4:
	echo ' <link rel="stylesheet" type="text/css" href="style/orange.css" />';
}
?>


  <script type="text/javascript" src="ajaxcalls.js"></script>
</head>

<body onload="show('home.html', 'column2', '1');">
  <div id="main">
  
  
    <div id="links">
      <!-- **** INSERT LINKS HERE **** -->
      <a href="#">Home</a> | <a href="#">LUG Home</a> | <a href="#">Site Map</a>
    </div>
    
    <div id="logo">
    
    <table>
    <tr>
    <td>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <embed height="72px" width="350px" src="style/mukti_banner.swf">
    <h2>&quot;National level symposium on Linux and Opensource&quot;</h2>
    </td>
    <td>
    </td>
    </tr>    
    </table>

    
    </div>
    
    <div id="menu">
      <ul id="banner_menu">
        <!-- **** INSERT NAVIGATION ITEMS HERE (use id="selected" to identify the page you're on **** -->
        <li><a id="1" href="javascript:void(0);" onclick="show('home.html','column2', '1');">home</a></li>
        <li><a id="2" href="javascript:void(0);" onclick="show('events.html','column2', '2');">Events</a></li>
        <li><a id="3" href="javascript:void(0);" onclick="show('','column2', '3');">Registration</a></li>
        <li><a id="4" href="javascript:void(0);" onclick="show('','column2', '4');">Schedule</a></li>
        <li><a id="5" href="javascript:void(0);" onclick="show('contact.html','column2', '5');">contact</a></li>
      </ul>
    </div>    
<script>var prev='1';document.getElementById('1').id='selected';</script>   



    <div id="content">
      <div id="column1">
  

    <!--
      
        <div class="sidebaritem">
          <h1>Sponsors</h1>
          <div class="sponsors">
          <table border="2" bordercolor="#C0C0C0" cellspacing="1"><tr>
			<td bgcolor="#FFFFFF" bordercolor="#C0C0C0">
          <SCRIPT src="sponsors.js" type=text/javascript></SCRIPT>
          </td></tr></table>
          </div>
          
          
          <h2>01.09.2006</h2>
          <p>This is where you can put your latest news.</p>
          <p><a href="#">read more ...</a></p>
          <h2>01.09.2006</h2>
          <p>This is where you can put your latest news.</p>
          <p><a href="#">read more ...</a></p>
          
        </div>
        
        
        -->
        
        
        <div class="sidebaritem">
          <h1>Events</h1>
          <div class="sbilinks">
            <!-- **** INSERT ADDITIONAL LINKS HERE **** -->
            <ul>
              <li><a href="javascript:void(0);" onclick="show('incanity.html','column2', '2');">inCanity</a></li>
              <li><a href="javascript:void(0);" onclick="show('konfigure.html','column2', '2');">Kon-figure</a></li>
              <li><a href="javascript:void(0);" onclick="show('shristi.html','column2', '2');">Srishti</a></li>
              <li><a href="javascript:void(0);" onclick="show('kaffeinewars.html','column2', '2');">Kaffeine Wars</a></li>
              <li><a href="javascript:void(0);" onclick="show('brainmesh.html','column2', '2');">Brain mesh</a></li>
              <li><a href="javascript:void(0);" onclick="show('codecracker.html','column2', '2');">Code Cracker</a></li>
              <li><a href="javascript:void(0);" onclick="show('jigyasa.html','column2', '2');">Jigyasa</a></li>
              <li><a href="javascript:void(0);" onclick="show('anusandhan.html','column2', '2');">Anusandhan</a></li>
              <li><a href="javascript:void(0);" onclick="show('ankur.html','column2', '2');">Ankur</a></li>
              <li><a href="javascript:void(0);" onclick="show('rushhour.html','column2', '2');">Rush Hour</a></li>
              <li><a href="javascript:void(0);" onclick="show('imagimp.html','column2', '2');">imaGIMP</a></li>
              <li><a href="javascript:void(0);" onclick="show('ncore.html','column2', '2');">n-CORE</a></li>
              <li><a href="javascript:void(0);" onclick="show('gurukool.html','column2', '2');">GuRu KooL</a></li>
              <li><a href="javascript:void(0);" onclick="show('freakygeeks.html','column2', '2');">Freaky Geeks</a></li>       
            </ul>
          </div>
        </div>
        
        
        
        
        
        <div class="sidebaritem">
          <h1>Links</h1>
          <div class="sbilinks">
            <ul>
              <li><a href="www.nitdgp.ac.in">NIT Durgapur</a></li>
              <li><a href="http://www.w3schools.com/xhtml/default.asp">LUG NIT-Dgp</a></li>
              <li><a href="http://www.w3schools.com/css/default.asp">News</a></li>
              <li><a href="http://www.mozilla.com/firefox">Downloads</a></li>
            </ul>
          </div>
        </div>
      </div>
      
      
      
      <div id="column2">      
        <!-- All contents come here-->    
      </div>
      
      
      
      
    </div>
    <div id="footer">
	      <a href="http://www.lug.nitdgp.ac.in">copyright &copy; 2007 Linux Users' Group, NIT Durgapur</a> | <a href="mailto:webmaster@lug.nitdgp.ac.in">webmaster@lug.nitdgp.ac.in</a> | <a href="www.lug.nitdgp.ac.in/?page=whoswho.html">design by LUG-NITDgp</a>
    </div>
  </div>
</body>
</html>
