////////////////////////////
function SendBugReport_Init()
{
	DKDEBUGFUNC();
	DKCreate("DKDebug/SendBugReport.css");
	DKCreate("DKDebug/SendBugReport.html");
	DKAddEvent("SendBugReport_Button", "click", SendBugReport_OnEvent);
}

////////////////////////////
function SendBugReport_End()
{
	DKDEBUGFUNC();
	DKRemoveEvents(SendBugReport_OnEvent);
	DKClose("DKDebug/SendBugReport.html");
	DKClose("DKDebug/SendBugReport.css");
}

/////////////////////////////////////
function SendBugReport_OnEvent(event)
{
	DKDEBUGFUNC(event);
	DKDEBUG("SendBugReport_OnEvent("+DK_GetId(event)+","+DK_GetType(event)+","+DK_GetValue(event)+")\n");
	if(DK_Id(event, "SendBugReport_Button")){
		SendBugReport_CreateReport();
	}
}

/////////////////////////////////////
function SendBugReport_CreateReport()
{
	DKDEBUGFUNC();
	var str = DKWidget_GetValue("SendBugReport_Textarea");
	DKINFO(str+"\n");
	
	var assets = DKAssets_LocalAssets();
	var log = DKFile_FileToString(assets+"log.txt");
	DKFile_StringToFile(str+"\n\n"+log, assets+"log.txt");
	SendBugReport_SendReport();
	
	DKWidget_Hide("DKDebug/SendBugReport.html");
	//TODO: say thank you here
	DKCreate("DKMessage/DKMessage.js", function(){
		DKFrame_Widget("DKMessage/DKMessage.html");
		DKMessageBox_Message("Thank You :)");	
		DKFrame_Close("DKDebug/SendBugReport.html");
	});
}

///////////////////////////////////
function SendBugReport_SendReport()
{
	DKDEBUGFUNC();
	DKWidget_Show("SendBugReport_Image");
	DKWidget_Show("SendBugReport_Sending");
	DK_DoFrame();
	
	var num = Math.floor((Math.random() * 99999999) + 1);
	DKDebug_SendBugReport(num.toString());
}