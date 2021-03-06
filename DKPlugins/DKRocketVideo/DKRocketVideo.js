/////////////////////////////
function DKRocketVideo_Init()
{
	DKDEBUGFUNC();
	DKCreate("DKRocketVideo/DKRocketVideo.html");
	DKAddEvent("DKRocketVideo_playpause", "click", DKRocketVideo_OnEvent);
	DKAddEvent("DKRocketVideo_position", "click", DKRocketVideo_OnEvent);
	DKAddEvent("DKRocketVideo_positiontrack", "click", DKRocketVideo_OnEvent);
	DKAddEvent("DKRocketVideo_speaker", "click", DKRocketVideo_OnEvent);
	DKAddEvent("DKRocketVideo_volume", "click", DKRocketVideo_OnEvent);
	DKAddEvent("DKRocketVideo_volumetrack", "click", DKRocketVideo_OnEvent);
}

////////////////////////////
function DKRocketVideo_End()
{
	DKDEBUGFUNC();
	DKRemoveEvents(DKRocketVideo_OnEvent);
	DKClose("DKRocketVideo/DKRocketVideo.html");
}

/////////////////////////////////////
function DKRocketVideo_OnEvent(event)
{
	DKDEBUGFUNC(event);
	if(DK_Id(event, "DKRocketVideo_playpause")){
		DKRocketVideo_playpause();
	}
	if(DK_Id(event, "DKRocketVideo_position")){
		DKRocketVideo_position();
	}
	if(DK_Id(event, "DKRocketVideo_positiontrack")){
		DKRocketVideo_positiontrack();
	}
	if(DK_Id(event, "DKRocketVideo_speaker", "click")){
		DKRocketVideo_speaker();
	}
	if(DK_Id(event, "DKRocketVideo_volume", "click")){
		DKRocketVideo_volume();
	}
	if(DK_Id(event, "DKRocketVideo_volumetrack", "click")){
		DKRocketVideo_volumetrack();
	}
}

//////////////////////////////////
function DKRocketVideo_playpause()
{
	DKDEBUGFUNC();
	//toggle play/pause
	var src = DKWidget_GetAttribute("DKRocketVideo_playpause", "src");
	//DKINFO(("DKRocketVideo_playpause(): src="+src+"\n");
	
	if(src.indexOf("play.png") != -1){
		DKWidget_SetAttribute("DKRocketVideo_playpause", "src", "DKRocketVideo/pause.png");
	}
	else{
		DKWidget_SetAttribute("DKRocketVideo_playpause", "src", "DKRocketVideo/play.png");
	}
}

/////////////////////////////////
function DKRocketVideo_position()
{
	DKDEBUGFUNC();
}

//////////////////////////////////////
function DKRocketVideo_positiontrack()
{
	DKDEBUGFUNC();
}

////////////////////////////////
function DKRocketVideo_speaker()
{
	DKDEBUGFUNC();
	//toggle volume/mute
	var src = DKWidget_GetAttribute("DKRocketVideo_speaker", "src");
	DKINFO(("DKRocketVideo_speaker(): src="+src+"\n");
	
	if(src.indexOf("mute.png") != -1){
		DKWidget_SetAttribute("DKRocketVideo_speaker", "src", "DKRocketVideo/volume3.png");
	}
	else{
		DKWidget_SetAttribute("DKRocketVideo_speaker", "src", "DKRocketVideo/mute.png");
	}
}

///////////////////////////////
function DKRocketVideo_volume()
{
	DKDEBUGFUNC();
}

////////////////////////////////////
function DKRocketVideo_volumetrack()
{
	DKDEBUGFUNC();
}