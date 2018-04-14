var steps = [];
var testindex = 0;
var loadInProgress = false; // This is set to true when a page is still loading
 
/********* SETTINGS BEGIN **************/
var webPage = require('webpage');
var page = webPage.create();
page.settings.userAgent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36';
page.settings.javascriptEnabled = true;
page.settings.loadImages = true; // Script is much faster with this field set to false
phantom.cookiesEnabled = true;
phantom.javascriptEnabled = true;
/********* SETTINGS END ****************/
 
console.log('All settings loaded, start with execution');
page.onConsoleMessage = function(msg) {
    console.log(msg);
};

// Steps to perform
steps = [
 
    function() {
        console.log('Step 1 - Open home page');
        page.open("http://192.168.183.201:9084/Kolkata27/WISHN/Home.jsp", function(status) {
	    if (status !== 'success') {
                console.log('FAIL to load the address');
	    }
	});
    },


    function() {
        console.log('Step 2 - Populate and submit the login form');
	page.evaluate(function() {
            document.login.Username.value="supratik_scn2"
            document.login.Password.value="wnpl123"
            document.getElementById("submit_btn").click();
	});
    },


    function() {
	console.log("Step 3 - Wait to login user");

	/*
        var fs = require('fs');
	var result = page.evaluate(function() {
	    return document.querySelectorAll("html")[0].outerHTML;
	});
        fs.write('LoggedIn.html',result,'w');
	*/
	page.render('result.png');
    },

];


// End Steps. Execute steps one by one
interval = setInterval(executeRequestsStepByStep,50);
 
function executeRequestsStepByStep(){
    if (loadInProgress == false && typeof steps[testindex] == "function") {
        console.log("step " + (testindex + 1));
        steps[testindex]();
        testindex++;
    }
    if (typeof steps[testindex] != "function") {
        console.log("test complete!");
        phantom.exit();
    }
}
 
/**
 * These listeners are very important in order to phantom work properly. 
 * Using these listeners, we control loadInProgress marker which controls. 
 * whether a page is fully loaded.
 * Without this, we will get content of the page, even a page is not fully loaded.
 */
page.onLoadStarted = function() {
    loadInProgress = true;
    console.log('Loading started');
};
page.onLoadFinished = function() {
    loadInProgress = false;
    console.log('Loading finished');
};
page.onConsoleMessage = function(msg) {
    console.log(msg);
};
