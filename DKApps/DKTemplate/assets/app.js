var USE_CEF = 1;     //Desktop
var USE_WEBVIEW = 0; //Android, iOS?
var USE_SDL = 0;     //Use with caution
var USE_ROCKET = 0;  //Use with caution
var DKApp_url = "file:///"+DKAssets_LocalAssets()+"index.html";
//var DKApp_url = "http://digitalknob.com/DKTemplate";

DKCreate("DK/init.js", function(){});

///////////////////////
function app_LoadPage()
{
	DKLog("app_LoadPage()\n");
	
	DKCreate("DKWindow/DKWindow.js", function(){
	DKCreate("DKScale/DKScale.js", function(){
	DKCreate("DKGui/DKFrame.js", function(){
	DKCreate("DKGui/DKMenu.js", function(){
	DKCreate("DKDebug/DKDebug.js", function(){
	DKCreate("DKEditor/DKEditor.js", function(){
	DKCreate("DKSDLText"); //SDL fps counter
	//DKCreate("DKHook/DKHook.js", function(){
		//DKFrame_Widget("DKHook/DKHook.html");
	//});		
	});
	});
	});
	});
	});
	});
}