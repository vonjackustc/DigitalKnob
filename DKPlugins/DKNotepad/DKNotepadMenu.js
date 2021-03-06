/////////////////////////////
function DKNotepadMenu_Init()
{
	DKDEBUGFUNC();
	DKCreate("DKNotepad/DKNotepadMenu.html");
	//DKAddEvent("window", "mousedown", DKNotepadMenu_OnEvent);
	DKAddEvent("DKNotepadMenu_Cut", "mousedown", DKNotepadMenu_OnEvent);
	DKAddEvent("DKNotepadMenu_Copy", "mousedown", DKNotepadMenu_OnEvent);
	DKAddEvent("DKNotepadMenu_Paste", "mousedown", DKNotepadMenu_OnEvent);
}

////////////////////////////
function DKNotepadMenu_End()
{
	DKDEBUGFUNC();
	DKRemoveEvents(DKNotepadMenu_OnEvent);
	DKClose("DKNotepad/DKNotepadMenu.html");
}

/////////////////////////////////////
function DKNotepadMenu_OnEvent(event)
{
	DKDEBUGFUNC(event);
	if(DK_Id(event,"DKNotepadMenu_Cut")){
		DKNotepadMenu_Cut();
		PreventDefault(event);
	}
	if(DK_Id(event,"DKNotepadMenu_Copy")){
		DKNotepadMenu_Copy();
	}
	if(DK_Id(event,"DKNotepadMenu_Paste")){
		DKNotepadMenu_Paste();
	}
	
	if(DK_Id(event, "window")){
		if(DKWidget_IsChildOf(DKWidget_GetHoverElement(), "DKNotepad/DKNotepadMenu.html")){
			return;
		}
	}
	DKClose("DKNotepad/DKNotepadMenu.js");
}

////////////////////////////
function DKNotepadMenu_Cut()
{
	DKDEBUGFUNC();
	DKWidget_Cut("DKNotepad_Text");
}

/////////////////////////////
function DKNotepadMenu_Copy()
{
	DKDEBUGFUNC();
	DKWidget_Copy("DKNotepad_Text");
}

//////////////////////////////
function DKNotepadMenu_Paste()
{
	DKDEBUGFUNC();
	DKWidget_Paste("DKNotepad_Text");
}