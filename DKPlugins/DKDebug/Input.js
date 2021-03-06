/////////////////////
function Input_Init()
{
	DKDEBUGFUNC();
	DKCreate("DKDebug/Input.html");
	DKAddEvent("Input_Text", "keydown", Input_OnEvent);
}

////////////////////
function Input_End()
{
	DKDEBUGFUNC();
	DKRemoveEvents(Input_OnEvent);
	DKClose("DKDebug/Input.html");
}

/////////////////////////////
function Input_OnEvent(event)
{
	DKDEBUGFUNC(event);
	DKDEBUG("Input_OnEvent("+DK_GetId(event)+","+DK_GetType(event)+","+DK_GetValue(event)+")\n");
	if(DK_Id(event, "Input_Text")){
		var key = DK_GetValue(event);
		//DKINFO("Input_Text: key="+key+"\n");
		if(key != 13 /*&& key != 72*/){ return; }
		Input_Run(DKWidget_GetValue("Input_Text"))
	}
}

///////////////////////////
function Input_Run(command)
{
	DKDEBUGFUNC(command);
	DK_RunDuktape(command);
}