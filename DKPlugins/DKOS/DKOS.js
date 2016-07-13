DKCreate("DKTray/DKTray.js");
DKCreate("DKRocket/DKBrowser.css");
DKCreate("DKWindow");
DKCreate("DKRocket");
var OS = DK_GetOS();
DKLog("OS="+OS+"\n");
if(OS != "IOS" && OS != "ANDROID"){
	var screenwidth = DK_GetScreenWidth();
	var screenheight = DK_GetScreenHeight();
	var newwidth = parseInt(screenwidth)-100;
	var newheight = parseInt(screenheight)-150;
	DKWindow_SetWidth(newwidth.toString()+"px");
	DKWindow_SetHeight(newheight.toString()+"px");
}

DKCreate("DKFrame/DKFrame.js")
DKCreate("DKOS/Desktop.js");
DKCreate("DKOS/Taskbar.js");
//DKCreate("DKDom");

if(OS == "IOS" || OS == "ANDROID"){
	DKWidget_SetScale(2);
}
