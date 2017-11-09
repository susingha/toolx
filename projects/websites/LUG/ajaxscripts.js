
function createRequestObject()
{ var xmlhttp = false;
        try
        {xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
        }
        catch (e)
         {try
                {
                        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
                  }
                catch (E)
                {
                        xmlhttp = false;
                }
         }
        if (!xmlhttp && typeof XMLHttpRequest!='undefined')
        {
                xmlhttp = new XMLHttpRequest();
        }
        return xmlhttp;
}



var loggedin = 0;

function authenticate(username, password)
{     
	var req = createRequestObject();
	req.open("POST", "verify.php?u="+username+"&p="+password ,true);
	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			loggedin = req.responseText;
//alert(loggedin);
/////////////////////////////////////////////--------------------------------
			if (loggedin != 0)
			{
				value = document.getElementById('t1').value;
				value2= document.getElementById('t2').value;

				document.getElementById('l1').innerHTML=step1;
				document.getElementById('l2').innerHTML=step2;
				document.getElementById('l3').innerHTML=step4;


				document.getElementById('t1').value = value;
				document.getElementById('t2').value = value2;
				l1 = 's1';
				l2 = 's2';
				l3 = 's4';

				document.form1.text3.focus();
				document.form1.text3.focus();

				showusername();
				showpage("login/home.php");
			}
			else
			{	

				value = document.getElementById('t2').value;	

				document.getElementById('l1').innerHTML=step2;
				document.getElementById('l2').innerHTML=step3;
				document.getElementById('l3').innerHTML=step1;

				document.getElementById('t2').value = value;
				l1 = 's2';
				l2 = 's3';
				l3 = 's1';
			
				document.form1.text1.focus();
				document.form1.text1.focus();
			}	
/////////////////////////////////////////////--------------------------------

		}
	}

	req.send(null);        
}

function logout()
{
	var req = createRequestObject();
	req.open("GET", 'logout.php', true);	
	
	req.send(null);
}


/*
function send_prompt(prompt)
{
	var req = createRequestObject();
	var params = 'prompt=' + prompt;
	req.open("POST", 'storeprompt.php', true);	
	req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	req.setRequestHeader("Content-length", params.length);
	req.setRequestHeader("Connection", "close"); 

	req.send(params);        	
}
*/


function showpage(link)
{
	showloading();
	var req = createRequestObject();
	req.open("GET", link, true);
	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			var response = req.responseText;
			document.getElementById('contents').innerHTML = response;
			hideloading();
		}
	}

	req.send(null);        
}





function check_availability()
{
	showloading();
	var req = createRequestObject();
	req.open("GET", 'check.php?username=' + document.getElementById('username').value, true);
	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			var response = req.responseText;
			hideloading();
			if(response == '1')
			{
				alert("The Username: " + document.getElementById('username').value + " is available.");
			}
			else
			{
				alert("The Username: " + document.getElementById('username').value + " is NOT available.");

			}
		}
	}

	req.send(null);        	
}



function showusername()
{
	var req = createRequestObject();
	req.open("GET", 'getusername.php', true);
	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			var response = req.responseText;
			document.getElementById('logname').innerHTML = "Welcome " + response;
		}
	}

	req.send(null);        	
}




function uploadwrittenarticle()
{
	showloading();
	var req = createRequestObject();
	var params = 'topic=' + document.getElementById('articletopic').value + '&catch=' + document.getElementById('articlecatch').value + '&content=' + document.getElementById('content').value;
	req.open("POST", "login/uploadwrittenarticle.php", true);
	req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	req.setRequestHeader("Content-length", params.length);
	req.setRequestHeader("Connection", "close"); 

	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			var response = req.responseText;
			document.getElementById('contents').innerHTML = response;
			hideloading();
		}
	}

	req.send(params);
}


//////////////////////////////////////////no more ajax//////////////////////////////////////////

















var line1 = document.getElementById('l1');
var line2 = document.getElementById('l2');
var line3 = document.getElementById('l3');

var text1 = document.getElementById('t1');
var text2 = document.getElementById('t2');
var text3 = document.getElementById('t3');

var step1 = '<font color="#B7B7B7" size="2">Linux Login&nbsp;-&nbsp;Username&nbsp;:</font><input type="text" name="text1" class="hiddentext" size="25" id="t1"  onkeypress="usernameentered(event.which)">';
var step2 = '<font color="#B7B7B7" size="2">Password&nbsp;:</font><input type="password"  name="text2" class="hiddenpass" size="25" id="t2" onkeypress="passwordentered(event.which)">';
var step3 = '<font color="#B7B7B7" size="2">Username&nbsp;and&nbsp;Password&nbsp;do&nbsp;not&nbsp;match</font>';
var step4 = '<font color="#B7B7B7" size="2">linux@localhost&nbsp;#</font><input type="text"  name="text3" class="hiddentext" size="25" id="t3" onkeypress="commandentered(event.which)">';
var step5 = '<font color="#B7B7B7" size="2">linux@localhost&nbsp;#</font><input type="text"  name="text4" class="hiddentext" size="25" id="t4" onkeypress="commandentered(event.which)">';
var step6 = '&nbsp;:&nbsp;command not found</font>';


var l1 = 's1';
var l2 = 's2';
var l3 = 's3';

var value = "";
var value2= "";



function usernameentered(key)
{	
	if ( ! key )
	{
		key = event.keyCode;
	}

	if(key == 13)
	{
//		alert(key);

		if     (l1 == 's1')
		{	
			document.getElementById('l2').innerHTML=step2;
			
		}
		else if(l3 == 's1')
		{
			value = document.getElementById('t1').value;

			document.getElementById('l1').innerHTML=step3;
			document.getElementById('l2').innerHTML=step1;
			document.getElementById('l3').innerHTML=step2;

			document.getElementById('t1').value = value;
			l1 = 's3';
			l2 = 's1';
			l3 = 's2';	
		}

		document.form1.text2.focus();
		document.form1.text2.focus();
		
	}
}



function passwordentered(key)
{

	if ( ! key )
	{
		key = event.keyCode;
	}

	if(key == 13)
	{
//		alert(key);
		authenticate(document.getElementById('t1').value, document.getElementById('t2').value);		
	}
}


function commandentered(key)
{
	if ( ! key )
	{
		key = event.keyCode;
	}

	if(key == 13)
	{
		if(document.getElementById('t3').value == "logout" || document.getElementById('t3').value == "exit")
		{
			logout();
			showpage("logoutpage.html");
			document.getElementById('logname').innerHTML = "Welcome Guest";

			document.getElementById('l1').innerHTML=step1;
			document.getElementById('l2').innerHTML="&nbsp;";
			document.getElementById('l3').innerHTML="&nbsp;";
			l1 = 's1';
			l2 = 's2';
			l3 = 's3';
			document.form1.text1.focus();
			document.form1.text1.focus();
		}
		else if(document.getElementById('t3').value == "")
		{
			document.getElementById('l1').innerHTML=document.getElementById('l2').innerHTML;
			document.getElementById('l2').innerHTML=step5;
			document.getElementById('l3').innerHTML=step4;

			document.form1.text3.focus();
			document.form1.text3.focus();
		}
		else
		{
			value = document.getElementById('t3').value;

			document.getElementById('l1').innerHTML=step5;
			document.getElementById('l2').innerHTML='<font color="#B7B7B7" size="2">'+value+step6;
			document.getElementById('l3').innerHTML=step4;

			document.getElementById('t4').value = value;
			document.form1.text3.focus();
			document.form1.text3.focus();

		}

	//send_prompt(document.getElementById('prompt').innerHTML);

	}

}

function stopsubmit()
{
	return false;
}


function validate(id1,id2,id3,id4,id5)
{
	if(passwdmatch() == false || document.getElementById(id1).value == "" || document.getElementById(id2).value == "" || document.getElementById(id3).value == "" || document.getElementById(id4).value == "")
	{
		alert("Please check that you have entered the fields : 1)Username 2)Password 4)From 3)About Linux");
		return false;
	}
	else
	{
		return true;
	}
}


function passwdmatch()
{
	if(document.getElementById('pass').value == document.getElementById('repass').value)	
	{
		return true;
	}
	else
	{
		alert("Your passwords do not match");
		return false;
	}
}



var j = 0;
function toggletxtarea()
{
	if(j == 0)
	{
			document.getElementById('txtarea').style.display="inline";
			document.getElementById('toggler').innerHTML="- hide textarea";
	}
	else
	{
			document.getElementById('txtarea').style.display="none";
			document.getElementById('toggler').innerHTML="+ show textarea";
	}
	j = 1 - j;
}



function showloading()
{
	document.getElementById('loading').style.display = "inline";
}
function hideloading()
{
	document.getElementById('loading').style.display = "none";
}



function Gsitesearch(curobj)
{
var domainroot=curobj.domainroot[curobj.domainroot.selectedIndex].value
curobj.q.value="site:"+domainroot+" "+curobj.qfront.value
}

