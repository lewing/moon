var TestLogger = null;
var TestHost = null;
var TestPluginReady = false;
var pendingTypes = new Array ();
var pendingMessages = new Array ();
var ShutdownRequested = false;

var media_server_timeout;

// This file requires moon/test/js/js/utils.js

function assertForMediaServer ()
{
	// Got this from: http://www.faqts.com/knowledge_base/view.phtml/aid/1799/fid/124
	try {
		var img = new Image ();
		img.onload = mediaServerFound;
		img.onerror = mediaServerError;
		img.src = "http://moonlightmedia:81/img.bmp";
		media_server_timeout = setTimeout ("TestLogger.LogError ('Checking for media server existence timed out'); mediaServerError ();", 10000);
	} catch (ex) {
		TestLogger.LogError ("checkForMediaServer (): exception: " + ex);
		mediaServerError ();
	}
}

function mediaServerFound ()
{
	clearTimeout (media_server_timeout);
	TestLogger.LogDebug ("mediaServerFound (): Found the media server.");
}

function mediaServerError ()
{
	TestLogger.LogError ("Couldn't find the media server.");
	TestLogger.LogResult (-1);
	SignalShutdown ();
}

function checkForMediaServerResult ()
{
	TestLogger.LogDebug ("checkForMediaServerResult (), readyState: " + media_server_request.readyState);
}

function createTestPlugin ()
{
    if(document.body == null) {
        window.setTimeout("createTestPlugin ();", 10);
		return;
    }

	if (TestHost)
		return;

    // Create plugin html.
    var pluginHtml = "";

    if((navigator.appVersion.indexOf('MSIE') != -1)) {
        pluginHtml = 
            '<object id="_TestPlugin" width="0" height="0" ' +
                'classid = "CLSID:596F7B43-899C-42F4-BF3C-B62BA99E73FF">' +
            '<\/object>';
    } else {
        pluginHtml = 
            '<embed id="_TestPlugin" width="0" height="0" ' +
                'type="application\/x-jolttest">' +
            '<\/embed>';
    }

    // Insert plugin html into document.
    var obj = document.createElement("DIV");
    obj.innerHTML = pluginHtml;
    document.body.appendChild(obj);    
    
    // Wait until the inserted html has been loaded before accessing the plugin.
    window.setTimeout("loadTestPlugin ();", 10);
}

function createMockTestPlugin ()
{
	TestHost = new function ()
	{
		this.Connect = function () {};
		this.LogDebug = function (msg) { console.log ("DEBUG: " + msg); }
		this.LogError = function (msg) { console.log ("ERROR: " + msg); }
		this.LogWarning = function (msg) { console.log ("WARNING: " + msg); }
		this.LogResult = function (result) { console.log ("LOGRESULT: " + result); }
		this.TryLogResult = function (result) { console.log ("TRYLOGRESULT: " + result); }
		this.LogMessage = function (msg) { console.log ("MESSAGE: " + msg); }
		this.SignalShutdown = function  () { console.log ("TestHost.SignalShutdown (), please press the big X"); }
		this.CaptureSingleImage = function (a, b, c, d, e, f) { console.log ("Smile!"); }
		this.CaptureMultipleImages = function (a, b, c, d, e, f, g, h, i) { console.log ("Laugh!"); }
	};
}

function loadTestPlugin ()
{
	if (TestHost)
		return;
	
	TestHost = document.getElementById ("_TestPlugin");
	if (!TestHost) {
		window.setTimeout ("loadTestPlugin ();", 10);
	} else {
		TestHost.Connect ();
		TestLogger = TestHost;
		TestPluginReady = true;
		try {
			TestLogger.LogDebug ("Test plugin initialized");

			for (var i = 0; i < pendingTypes.length; i++) {
				switch (pendingTypes [i]) {
				case "Debug": TestLogger.LogDebug ("<delayed>: " + pendingMessages [i]); break;
				case "Error": TestLogger.LogError ("<delayed>: " + pendingMessages [i]); break;
				case "Warning": TestLogger.LogWarning ("<delayed>: " + pendingMessages [i]); break;
				case "Result": TestLogger.LogResult (pendingMessages [i]); break;
				case "TryResult": TestLogger.TryLogResult (pendingMessages [i]); break;
				case "Message": TestLogger.LogMessage ("<delayed>: " + pendingMessages [i]); break;
				}
			}
			OnTestPluginReady ();
		} catch (ex) {
		}
	}
}

function FinishTest ()
{
	TestHost.SignalShutdown (document.name);
}

function TakeSingleSnapshotAndShutdown (control, file_name, width, height, initial_delay)
{
	if (initial_delay) {
		window.setTimeout (function () {
				TakeSingleSnapshotAndShutdown (control, file_name, width, height, 0);
			}, initial_delay);
		return;
	}
	TestHost.CaptureSingleImage ("", file_name, 0, 0, width, height);
	FinishTest ();
}

function TakeMultipleSnapshotsAndShutdown (control, max_images_to_capture, capture_interval, initial_delay, width, height)
{
	TestHost.CaptureMultipleImages ("", "", 0, 0, width, height, max_images_to_capture, capture_interval, initial_delay);
	FinishTest ();
}

TestLogger = new function()
{
    this.LogMessage = function (msg)
    { 
        if (TestHost == null) {
			pendingMessages.push (msg); pendingTypes.push ("Message");
		} else {
		 	TestHost.LogMessage (msg);
		}
    }
    this.LogDebug = function (msg)
    { 
        if (TestHost == null) {
			pendingMessages.push (msg); pendingTypes.push ("Debug");
		} else {
		 	TestHost.LogDebug (msg);
		}
    }
    this.LogWarning = function (msg)
    { 
        if (TestHost == null) {
			pendingMessages.push (msg); pendingTypes.push ("Warning");
		} else {
		 	TestHost.LogWarning (msg);
		}
    }
    this.LogError = function (msg)
    { 
        if (TestHost == null) {
			pendingMessages.push (msg); pendingTypes.push ("Error");
		} else {
		 	TestHost.LogError (msg);
		}
    }
    this.LogResult = function (result)
    { 
        if (TestHost == null) {
			pendingMessages.push (result); pendingTypes.push ("Result");
		} else {
		 	TestHost.LogResult (result);
		}
    }
    this.TryLogResult = function (result)
    {
        if (TestHost == null) {
			pendingMessages.push (result); pendingTypes.push ("TryResult");
		} else {
		 	TestHost.TryLogResult (result);
		}
    }
}

function SignalShutdown ()
{
	if (!TestHost) {
		setTimeout ("SignalShutdown ();", 100);
		return;
	}

	ShutdownRequested = true;
	TestHost.SignalShutdown (document.name);
}

function createSafePlugin ()
{
	if (Host.Firefox && Host.Windows)
		createMockTestPlugin ();
	else
		createTestPlugin ();
}


window.setTimeout("createSafePlugin ();", 10);


