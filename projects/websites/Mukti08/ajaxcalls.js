
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



function show(link, div, n)
{
	alert(link + div);
	bannercolor(n);
	var req = createRequestObject();
	req.open("GET", link, true);
	req.onreadystatechange = function()
	{
		if(req.readyState == 4)
		{
			var response = req.responseText;
			document.getElementById(div).innerHTML = response;

		}
	}

	req.send(null);        
}


function bannercolor(n)
{
//alert("statr");	
	
	document.getElementById('selected').id=prev;
	document.getElementById(n).id='selected';
	prev=n;
	
//alert(document.getElementById('banner_menu').childNodes[n].childNodes[0].id='selected');
}
