//https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

#ifdef USE_DKDuktape 
#include "DK/DKApp.h"
#include "DK/DKFile.h"
#include "DK/DKOsInfo.h"
#include "DK/DKPlugin.h"
#include "DK/DKUnix.h"
#include "DK/DKUtil.h"
#include "DKDuktape/DKDuktapeJS.h"
#include <signal.h>

////////////////////////
bool DKDuktapeJS::Init()
{
	DKDEBUGFUNC();
	DKClass::DKCreate("DKFileJS");
	DKClass::DKCreate("DKAssetsJS");
	
	/*
	DKString errors;
	DKFile::GetSetting(DKFile::local_assets+"settings.txt", "[LOG_ERRORS]", errors);
	DKString warnings;
	DKFile::GetSetting(DKFile::local_assets+"settings.txt", "[LOG_WARNINGS]", warnings);
	DKString info;
	DKFile::GetSetting(DKFile::local_assets+"settings.txt", "[LOG_INFO]", info);
	DKString debug;
	DKFile::GetSetting(DKFile::local_assets+"settings.txt", "[LOG_DEBUG]", debug);
	DKString file;
	DKFile::GetSetting(DKFile::local_assets+"settings.txt", "[LOG_FILE]", file);
	if(same(errors,"OFF")){ log_errors = false; }
	if(same(warnings,"OFF")){ log_warnings = false; }
	if(same(info,"OFF")){ log_info = false; }
	if(same(debug,"OFF")){ log_debug = false; }
	if(same(file,"ON")){ log_file = true; }
	*/
	
	//duk_eval_string(DKDuktape::ctx, "var DK_ERROR = 0;");
	//duk_eval_string(DKDuktape::ctx, "var DK_WARN = 1;");
	//duk_eval_string(DKDuktape::ctx, "var DK_INFO = 2;");
	//duk_eval_string(DKDuktape::ctx, "var DK_DEBUG = 3;");

	DKDuktape::AttachFunction("DKDEBUGFUNC", DKDuktapeJS::_DKDEBUGFUNC);
	DKDuktape::AttachFunction("DKDEBUGVARS", DKDuktapeJS::_DKDEBUGVARS);
	DKDuktape::AttachFunction("DKAddEvent", DKDuktapeJS::_DKAddEvent);
	DKDuktape::AttachFunction("DKAvailable", DKDuktapeJS::_DKAvailable);
	DKDuktape::AttachFunction("DKClose", DKDuktapeJS::_DKClose);
	DKDuktape::AttachFunction("DKCreate", DKDuktapeJS::_DKCreate); //This will be overwritten by DK.js
	DKDuktape::AttachFunction("DKDuktape_Create", DKDuktapeJS::_DKCreate);
	DKDuktape::AttachFunction("DKLoadPlugin", DKDuktapeJS::_DKLoadPlugin);
	DKDuktape::AttachFunction("DKDuktape_RemoveEvent", DKDuktapeJS::_DKRemoveEvent);
	DKDuktape::AttachFunction("DKDuktape_RemoveEvents", DKDuktapeJS::_DKRemoveEvents);
	DKDuktape::AttachFunction("DKDuktape_SendEvent", DKDuktapeJS::_DKSendEvent);
	DKDuktape::AttachFunction("DK_Valid", DKDuktapeJS::_DKValid);

	DKDuktape::AttachFunction("DK_Beep", DKDuktapeJS::Beep);
	DKDuktape::AttachFunction("DK_CallFunc", DKDuktapeJS::CallFunc);
	DKDuktape::AttachFunction("DK_ClearEvents", DKDuktapeJS::ClearEvents);
	DKDuktape::AttachFunction("DK_ClickImage", DKDuktapeJS::ClickImage);
	DKDuktape::AttachFunction("DK_CpuUsed", DKDuktapeJS::CpuUsed);
	DKDuktape::AttachFunction("DK_CpuUsedByApp", DKDuktapeJS::CpuUsedByApp);
	DKDuktape::AttachFunction("DK_Crash", DKDuktapeJS::Crash);
	DKDuktape::AttachFunction("DK_DoFrame", DKDuktapeJS::DoFrame);
	DKDuktape::AttachFunction("DK_DoubleClick", DKDuktapeJS::DoubleClick);
	DKDuktape::AttachFunction("DK_Execute", DKDuktapeJS::Execute);
	DKDuktape::AttachFunction("DK_Exit", DKDuktapeJS::Exit);
	DKDuktape::AttachFunction("DK_GetArgs", DKDuktapeJS::GetArgs);
	DKDuktape::AttachFunction("DK_GetBrowser", DKDuktapeJS::GetBrowser);
	DKDuktape::AttachFunction("DK_GetClipboard", DKDuktapeJS::GetClipboard);
	DKDuktape::AttachFunction("DK_GetData", DKDuktapeJS::GetData);
	DKDuktape::AttachFunction("DK_GetDate", DKDuktapeJS::GetDate);
	DKDuktape::AttachFunction("DK_GetEvents", DKDuktapeJS::GetEvents);
	DKDuktape::AttachFunction("DK_GetFps", DKDuktapeJS::GetFps);
	DKDuktape::AttachFunction("DK_GetFramerate", DKDuktapeJS::GetFramerate);
	DKDuktape::AttachFunction("DK_GetFrames", DKDuktapeJS::GetFrames);
	DKDuktape::AttachFunction("DK_GetFunctions", DKDuktapeJS::GetFunctions);
	DKDuktape::AttachFunction("DK_GetId", DKDuktapeJS::GetId);
	DKDuktape::AttachFunction("DK_GetJSEngine", DKDuktapeJS::GetJSEngine);
	DKDuktape::AttachFunction("DK_GetKey", DKDuktapeJS::GetKey);
	DKDuktape::AttachFunction("DK_GetLocalIP", DKDuktapeJS::GetLocalIP);
	DKDuktape::AttachFunction("DK_GetMouseX", DKDuktapeJS::GetMouseX); 
	DKDuktape::AttachFunction("DK_GetMouseY", DKDuktapeJS::GetMouseY);
	DKDuktape::AttachFunction("DK_GetOS", DKDuktapeJS::GetOS);
	DKDuktape::AttachFunction("DK_GetObjects", DKDuktapeJS::GetObjects);
	DKDuktape::AttachFunction("DK_GetPixelUnderMouse", DKDuktapeJS::GetPixelUnderMouse);
	DKDuktape::AttachFunction("DK_GetProcessList", DKDuktapeJS::GetProcessList);
	DKDuktape::AttachFunction("DK_GetScreenHeight", DKDuktapeJS::GetScreenHeight);
	DKDuktape::AttachFunction("DK_GetScreenWidth", DKDuktapeJS::GetScreenWidth);
	DKDuktape::AttachFunction("DK_GetTicks", DKDuktapeJS::GetTicks);
	DKDuktape::AttachFunction("DK_GetTime", DKDuktapeJS::GetTime);
	DKDuktape::AttachFunction("DK_GetType", DKDuktapeJS::GetType);
	DKDuktape::AttachFunction("DK_GetUsername", DKDuktapeJS::GetUsername);
	DKDuktape::AttachFunction("DKDuktape_GetValue", DKDuktapeJS::GetValue);
	DKDuktape::AttachFunction("DK_GetVolume", DKDuktapeJS::GetVolume);
	DKDuktape::AttachFunction("DK_HideConsole", DKDuktapeJS::HideConsole);
	DKDuktape::AttachFunction("DK_IE", DKDuktapeJS::IE);
	DKDuktape::AttachFunction("DK_Include", DKDuktapeJS::Include);
	DKDuktape::AttachFunction("DK_KeyIsDown", DKDuktapeJS::KeyIsDown);
	DKDuktape::AttachFunction("DK_LeftClick", DKDuktapeJS::LeftClick);
	DKDuktape::AttachFunction("DK_LogGuiConsole", DKDuktapeJS::LogGuiConsole);
	DKDuktape::AttachFunction("DK_LowPowerMonitor", DKDuktapeJS::LowPowerMonitor);
	DKDuktape::AttachFunction("DK_MessageBox", DKDuktapeJS::MessageBox);
	DKDuktape::AttachFunction("DK_MouseToImage", DKDuktapeJS::MouseToImage);
	DKDuktape::AttachFunction("DK_PhysicalMemory", DKDuktapeJS::PhysicalMemory);
	DKDuktape::AttachFunction("DK_PhysicalMemoryUsed", DKDuktapeJS::PhysicalMemoryUsed);
	DKDuktape::AttachFunction("DK_PhysicalMemoryUsedByApp", DKDuktapeJS::PhysicalMemoryUsedByApp);
	DKDuktape::AttachFunction("DK_PressKey", DKDuktapeJS::PressKey);
	DKDuktape::AttachFunction("DK_QueueDuktape", DKDuktapeJS::QueueDuktape);
	DKDuktape::AttachFunction("DK_ReleaseKey", DKDuktapeJS::ReleaseKey);
	DKDuktape::AttachFunction("DK_Reload", DKDuktapeJS::Reload);
	DKDuktape::AttachFunction("DK_RightClick", DKDuktapeJS::RightClick);
	DKDuktape::AttachFunction("DK_Run", DKDuktapeJS::Run);
	DKDuktape::AttachFunction("DK_RunDuktape", DKDuktapeJS::RunDuktape);
	DKDuktape::AttachFunction("DK_SetBrightness", DKDuktapeJS::SetBrightness);
	DKDuktape::AttachFunction("DK_SetClipboard", DKDuktapeJS::SetClipboard);
	DKDuktape::AttachFunction("DK_SetClipboardFiles", DKDuktapeJS::SetClipboardFiles);
	DKDuktape::AttachFunction("DK_SetClipboardImage", DKDuktapeJS::SetClipboardImage);
	DKDuktape::AttachFunction("DK_SetCursorPos", DKDuktapeJS::SetCursorPos);
	DKDuktape::AttachFunction("DK_SetFramerate", DKDuktapeJS::SetFramerate);
	DKDuktape::AttachFunction("DK_SetLog", DKDuktapeJS::_SetLog);
	DKDuktape::AttachFunction("DK_SetVolume", DKDuktapeJS::SetVolume);
	DKDuktape::AttachFunction("DK_ShowConsole", DKDuktapeJS::ShowConsole);
	DKDuktape::AttachFunction("DK_Sleep", DKDuktapeJS::Sleep);
	DKDuktape::AttachFunction("DK_StopPropagation", DKDuktapeJS::StopPropagation);
	DKDuktape::AttachFunction("DK_StrokeKey", DKDuktapeJS::StrokeKey);
	DKDuktape::AttachFunction("DK_System", DKDuktapeJS::System);
	DKDuktape::AttachFunction("DK_TurnOffMonitor", DKDuktapeJS::TurnOffMonitor);
	DKDuktape::AttachFunction("DK_TurnOnMonitor", DKDuktapeJS::TurnOnMonitor);
	DKDuktape::AttachFunction("DK_Value", DKDuktapeJS::Value);
	DKDuktape::AttachFunction("DK_VirtualMemory", DKDuktapeJS::VirtualMemory);
	DKDuktape::AttachFunction("DK_VirtualMemoryUsed", DKDuktapeJS::VirtualMemoryUsed);
	DKDuktape::AttachFunction("DK_VirtualMemoryUsedByApp", DKDuktapeJS::VirtualMemoryUsedByApp);
	DKDuktape::AttachFunction("DK_WaitForImage", DKDuktapeJS::WaitForImage);
	return true;
}


//////////////////////////////////////////////
int DKDuktapeJS::_DKAddEvent(duk_context* ctx)
{
	DKString id = duk_require_string(ctx, 0);
	DKString type = duk_require_string(ctx, 1);

	DKString jsreturn;
	if(duk_to_string(ctx, 2)){
		jsreturn = duk_to_string(ctx, 2);
		replace(jsreturn, "function ", "");
	}

	DKEvent::AddEvent(id, type, jsreturn, &DKDuktape::OnEvent, DKDuktape::Get());
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::_DKAvailable(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	bool available = DKClass::DKAvailable(data);
	if(!available){
		return 0;
	}
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::_DKClose(duk_context* ctx)
{
	DKString value = duk_require_string(ctx, 0);
	DKClass::DKClose(value);
	return 1;
}

////////////////////////////////////////////
int DKDuktapeJS::_DKCreate(duk_context* ctx)
{	
	DKString data = duk_require_string(ctx, 0);
	
	bool callback_found = false;
	if(duk_is_function(ctx, -1)){
		callback_found = true;
	}
	
	DKObject* obj = DKClass::DKCreate(data);
	if(!obj){
		duk_push_string(ctx, "");
		DKWARN("DKDuktapeJS::_DKCreate("+data+"): obj invalid \n");
		return 1;
	}

	//Call the callback
	if(callback_found){
		if(duk_pcall(ctx, 0) != 0 && duk_pcall(ctx, 1) != 0){ // JsFunc call failed
			/*
			//TODO - can we get more info on the actuall callback to display here
			// callback name, parameters, inner code, etc.
			DKString error = toString(duk_safe_to_string(ctx, -1));
			replace(error, "'", "\\'");
			DKString str = "var err = new Error();";
			str += "DKERROR('########## DUKTAPE STACK TRACE ##########\\n";
			str += error+"\\n";
			str += "'+err.stack+'\\n');";
			duk_eval_string(ctx, str.c_str());
			*/

			duk_get_prop_string(ctx, -1, "name");  // push `err.name`
			DKString name = duk_get_string(ctx, -1);
			duk_pop(ctx);  // pop `err.name`
			duk_get_prop_string(ctx, -1, "message");  // push `err.message`
			DKString message = duk_get_string(ctx, -1);
			duk_pop(ctx);  // pop `err.message`
			message = name +": "+message;
			duk_get_prop_string(ctx, -1, "fileName");  // push `err.fileName`
			DKString fileName = duk_get_string(ctx, -1);
			duk_pop(ctx);  // pop `err.fileName`
			duk_get_prop_string(ctx, -1, "lineNumber");  // push `err.lineNumber`
			DKString lineNumber = toString(duk_get_int(ctx, -1));
			duk_pop(ctx);  // pop `err.lineNumber`
			duk_get_prop_string(ctx, -1, "stack");  // push `err.stack`
			DKString stack = duk_get_string(ctx, -1);
			duk_pop(ctx);  // pop `err.stack`

			DKERROR(message+"\n");

			replace(stack,"'","\\'");
			replace(stack,"\n","\\n");
			replace(message,"'","\\'");

			DKString str;
			str += "var err_error = {stack:'"+stack+"'};";
			str += "var err_event = {type:'error', message:'"+message+"', filename:'"+fileName+"', lineno:'"+lineNumber+"', colno:'0', error:err_error};";
			str += "EventFromCPP('window', err_event);";
			duk_eval_string(ctx, str.c_str());
		}
		duk_pop(ctx);
    }

	duk_push_string(ctx, obj->data[1].c_str());
	return 1;
}

/////////////////////////////////////////
int DKDuktapeJS::_DKLoadPlugin(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	DKPlugins::LoadPlugin(file);
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::_DKDEBUGFUNC(duk_context* ctx)
{
	//TODO - we need to pull the function name
	DKString str = "unknown_func(";
	int i=0;
	while(duk_is_valid_index(ctx, i)){
		if(duk_is_boolean(ctx, i)){ str += toString(duk_get_boolean(ctx, i)); }
		if(duk_is_number(ctx, i)){ str += toString(duk_get_number(ctx, i)); }
		if(duk_is_string(ctx, i)){ str += duk_get_string(ctx, i); }
		str += ","; //FIXME - no comma on the last argument
		i++;
	}
	str += ")";
	DKDEBUG(str+"\n");
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::_DKDEBUGVARS(duk_context* ctx)
{
	//TODO - we need to pull the function name and the variable names
	DKString info = "unknown_file:unknown_line unknown_func()   ";
	int i=0;
	while(duk_is_valid_index(ctx, i)){
		DKString var = "unknown_name: ";
		if(duk_is_boolean(ctx, i)){ var += toString(duk_get_boolean(ctx, i)); }
		if(duk_is_number(ctx, i)){ var += toString(duk_get_number(ctx, i)); }
		if(duk_is_string(ctx, i)){ var += duk_get_string(ctx, i); }
		if(!log_debug){
			log_debug = true;
			DKDEBUG(info+var+"\n");
			log_debug = false;
		}
		else{
			DKDEBUG(info+var+"\n");
		}
		i++;
	}
	return 1;
}

//////////////////////////////////////////
int DKDuktapeJS::_DKRemoveEvent(duk_context* ctx)
{
	DKString id = duk_require_string(ctx, 0);
	DKString type = duk_require_string(ctx, 1);
	DKString jsreturn;
	if(duk_to_string(ctx, 2)){
		jsreturn = duk_to_string(ctx, 2);
		replace(jsreturn, "function ", "");
	}

	if(!DKEvent::RemoveEvent(id, type, jsreturn)){ return 0; }
	return 1;
}

///////////////////////////////////////////
int DKDuktapeJS::_DKRemoveEvents(duk_context* ctx)
{
	//variable can be id or jsreturn
	DKString variable = duk_to_string(ctx, 0);
	replace(variable, "function ", ""); //jsreturn type
	if(!DKEvent::RemoveEvents(variable)){ return 0; } //sending id or jsreturn
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::_DKSendEvent(duk_context* ctx)
{
	DKWARN("DKDuktapeJS::_DKSendEvent()\n");
	DKString id = duk_require_string(ctx, 0);
	DKString type = duk_require_string(ctx, 1);
	DKString param;

	//if(duk_get_string(ctx, 1)){
	//	type = duk_get_string(ctx, 1);
	//}

	if(duk_get_string(ctx, 2)){
		param = duk_get_string(ctx, 2);
	}

	if(!param.empty()){
		DKEvent::SendEvent(id, type, param);
		return 1;
	}

	DKEvent::SendEvent(id, type, "");
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::_DKValid(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	bool valid = DKClass::DKValid(data);
	if(!valid){
		return 0;
	}
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::_SetLog(duk_context* ctx)
{
	//TODO
	int lvl = duk_require_int(ctx, 0);
	DKString string = duk_require_string(ctx, 1);
	SetLog(lvl, string);
	return 1;
}



////////////////////////////////
int DKDuktapeJS::Beep(duk_context* ctx)
{
	DKUtil::Beep();
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::CallFunc(duk_context* ctx)
{
	DKString func = duk_require_string(ctx, 0);
	DKString args = duk_require_string(ctx, 1);
	DKString result;

	if(!DKClass::CallFunc(func, &args, &result)){ return 0;	}
	
	duk_push_string(ctx, result.c_str());
	return 1;
}

///////////////////////////////////////
int DKDuktapeJS::ClearEvents(duk_context* ctx)
{
	DKEvent::events.clear();
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::ClickImage(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	int x;
	int y;
	if(!DKUtil::FindImageOnScreen(file, x, y)){ return 0; }
	if(!DKUtil::SetMousePos(x, y)){ return 0; }
	if(!DKUtil::LeftClick()){ return 0; }
	duk_push_true(ctx);
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::CpuUsed(duk_context* ctx)
{
	int cpu;
	if(!DKUtil::CpuUsed(cpu)){ return 0; }
	duk_push_number(ctx, cpu);
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::CpuUsedByApp(duk_context* ctx)
{
	int cpu;
	if(!DKUtil::CpuUsedByApp(cpu)){ return 0; }
	duk_push_number(ctx, cpu);
	return 1;
}

/////////////////////////////////
int DKDuktapeJS::Crash(duk_context* ctx)
{
#if !defined(WIN32)
	raise(SIGSEGV);
#else
	abort();
#endif
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::DoFrame(duk_context* ctx)
{
	DKApp::DoFrame();
	return 1;
}

///////////////////////////////////////
int DKDuktapeJS::DoubleClick(duk_context* ctx)
{
	if(!DKUtil::DoubleClick()){ return 0; }
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::Execute(duk_context* ctx)
{
	DKString command = duk_require_string(ctx, 0);
	DKString rtn;
	if(!DKUtil::Execute(command, rtn)){ return 0; }
	duk_push_string(ctx, rtn.c_str());
	return 1;
}

////////////////////////////////
int DKDuktapeJS::Exit(duk_context* ctx)
{
	DKApp::Exit();
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::GetArgs(duk_context* ctx)
{
	DKString args;
	if(DKApp::argc > 1){
		for(int i = 1; i < DKApp::argc; ++i){
			//DKINFO("argv["+toString(i)+"] = "+toString(DKApp::argv[i])+"\n");
			args += toString(DKApp::argv[i]) += ";";
		}
	}
	duk_push_string(ctx, args.c_str());
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::GetBrowser(duk_context* ctx)
{
	DKString browser = "Rocket";
	duk_push_string(ctx, browser.c_str());
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::GetClipboard(duk_context* ctx)
{
	DKString string;
	if(!DKUtil::GetClipboard(string)){ return 0; }
	duk_push_string(ctx, string.c_str());
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::GetData(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	if(!DKClass::DKValid(data)){ return 0; }
	DKString output = toString(DKClass::DKGet(data)->data, ",");
	duk_push_string(ctx, output.c_str());
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::GetDate(duk_context* ctx)
{
	DKString date;
	if(!DKUtil::GetDate(date)){ return 0; }
	duk_push_string(ctx, date.c_str());
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::GetEvents(duk_context* ctx)
{
	DKString list;
	for(unsigned int i = 0; i < DKEvent::events.size(); ++i){
			
		list += DKEvent::events[i]->GetId();
		list += " : ";
		list += DKEvent::events[i]->GetType();
		list += " : ";
		list += DKEvent::events[i]->GetJSReturn();
		list += ",";
	}

	duk_push_string(ctx, list.c_str());
	return 1;
}

//////////////////////////////////
int DKDuktapeJS::GetFps(duk_context* ctx)
{
	unsigned int fps;
	DKUtil::GetFps(fps);
	duk_push_int(ctx, fps);
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::GetFramerate(duk_context* ctx)
{
	int framerate;
	if(!DKUtil::GetFramerate(framerate)){ return 0; }
	duk_push_int(ctx, framerate);
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::GetFrames(duk_context* ctx)
{
	long frames;
	if(!DKUtil::GetFrames(frames)){ return 0; }
	duk_push_int(ctx, frames);
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::GetFunctions(duk_context* ctx)
{
	DKStringArray list;
	for(unsigned int i=0; i < DKDuktape::functions.size(); ++i){
		list.push_back(DKDuktape::functions[i]);
	}
	DKString str = toString(list, ",");
	duk_push_string(ctx, str.c_str());
	return 1;
}

/////////////////////////////////
int DKDuktapeJS::GetId(duk_context* ctx)
{
	DKString evt = duk_require_string(ctx, 0);
	DKStringArray arry;
	toStringArray(arry, evt, ",");
	if(arry.size() < 1){ return 0; }
	duk_push_string(ctx, arry[0].c_str());
	return 1;
}

//////////////////////////////////////////////
int DKDuktapeJS::GetJSEngine(duk_context* ctx)
{
	DKString engine = "Duktape";
	duk_push_string(ctx, engine.c_str());
	return 1;
}

//////////////////////////////////
int DKDuktapeJS::GetKey(duk_context* ctx)
{
	int key;
	if(!DKUtil::GetKey(key)){ return 0; }
	duk_push_int(ctx, key);
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::GetLocalIP(duk_context* ctx)
{
	DKString ip;
	if(!DKUtil::GetLocalIP(ip)){ return 0; }
	duk_push_string(ctx, ip.c_str());
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::GetMouseX(duk_context* ctx)
{
	int mouseX = 0;
	int mouseY = 0;
	if(!DKUtil::GetMousePos(mouseX, mouseY)){ return 0; }
	duk_push_int(ctx, mouseX);
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::GetMouseY(duk_context* ctx)
{
	int mouseX = 0;
	int mouseY = 0;
	if(!DKUtil::GetMousePos(mouseX, mouseY)){ return 0; }
	duk_push_int(ctx, mouseY);
	return 1;
}

/////////////////////////////////
int DKDuktapeJS::GetOS(duk_context* ctx)
{
	DKString os;
	if(!GetSystemOS(os)){
		return 0; 
	}
	duk_push_string(ctx, os.c_str());
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::GetObjects(duk_context* ctx)
{
	DKStringArray list;
	DKClass::GetObjects(list);
	for(unsigned int i=0; i<list.size(); ++i){
		replace(list[i], ",", " : ");
	}
	DKString str = toString(list, ",");
	duk_push_string(ctx, str.c_str());
	return 1;
}

//////////////////////////////////////////////
int DKDuktapeJS::GetPixelUnderMouse(duk_context* ctx)
{
	int mouseX = 0;
	int mouseY = 0;
	if (!DKUtil::GetMousePos(mouseX, mouseY)) { return 0; }
	int r;
	int g;
	int b;
	if (!DKUtil::GetPixelFromScreen(mouseX, mouseY, r, g, b)) { return 0; }
	DKString rgb = toString(r) + "," + toString(g) + "," + toString(b);
	duk_push_string(ctx, rgb.c_str());
	return 1;
}

//////////////////////////////////////////
int DKDuktapeJS::GetProcessList(duk_context* ctx)
{
	DKString list;
	DKUtil::GetProcessList(list);
	duk_push_string(ctx, list.c_str());
	return 1;
}

///////////////////////////////////////////
int DKDuktapeJS::GetScreenHeight(duk_context* ctx)
{
	int h;
	if(!DKUtil::GetScreenHeight(h)){ return 0; }
	duk_push_int(ctx, h);
	return 1;
}

//////////////////////////////////////////
int DKDuktapeJS::GetScreenWidth(duk_context* ctx)
{
	int w;
	if(!DKUtil::GetScreenWidth(w)){ return 0; }
	duk_push_int(ctx, w);
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::GetTicks(duk_context* ctx)
{
	long ticks;
	if(!DKUtil::GetTicks(ticks)){ return 0; }
	duk_push_int(ctx, ticks);
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::GetTime(duk_context* ctx)
{
	DKString time;
	if(!DKUtil::GetTime(time)){ return 0; }
	duk_push_string(ctx, time.c_str());
	return 1;
}

///////////////////////////////////
int DKDuktapeJS::GetType(duk_context* ctx)
{
	DKString evt = duk_require_string(ctx, 0);
	DKStringArray arry;
	toStringArray(arry, evt, ",");
	if(arry.size() < 2){ return 0; }
	duk_push_string(ctx, arry[1].c_str());
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::GetUsername(duk_context* ctx)
{
#ifdef LINUX
	DKString username;
	DKUnix::GetUsername(username);
	duk_push_string(ctx, username.c_str());
#else
	DKERROR("GetUsername: not implemented on this OS \n");
#endif
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::GetValue(duk_context* ctx)
{
	DKDEBUGFUNC(ctx);
	DKString evt = duk_require_string(ctx, 0);
	DKStringArray arry;
	toStringArray(arry, evt, ",");
	if(arry.size() < 3){ 
		if(DKClass::DKValid("DKWidget,DKWidget0")){
			DKString rval;
			DKDuktape::RunDuktape("DKWidget_GetValue(\""+evt+"\");", rval);
			if(rval.empty()){
				return 0;
			}
			duk_push_string(ctx, rval.c_str());
			return 1;
		}
		//DKERROR("DKDuktapeJS::GetValue(" + evt + "): failed\n");
		return 0;
	}
	duk_push_string(ctx, arry[2].c_str());
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::GetVolume(duk_context* ctx)
{
	int percent;
	if(!DKUtil::GetVolume(percent)){ return 0; }
	duk_push_number(ctx, percent);
	return 1;
}

///////////////////////////////////////
int DKDuktapeJS::HideConsole(duk_context* ctx)
{
#ifdef WIN32
	HWND consoleWindow = GetConsoleWindow();
	ShowWindow(consoleWindow, SW_HIDE);
#endif 
	return 1;
}

//////////////////////////////
int DKDuktapeJS::IE(duk_context* ctx)
{
	return 0;
}

///////////////////////////////////
int DKDuktapeJS::Include(duk_context* ctx)
{
	//ignore this for now
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::KeyIsDown(duk_context* ctx)
{
	int key = duk_require_int(ctx, 0);
	if(!DKUtil::KeyIsDown(key)){ return 0; }
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::LeftClick(duk_context* ctx)
{
	if(!DKUtil::LeftClick()){ return 0; }
	return 1;
}

/////////////////////////////////////////
int DKDuktapeJS::LogGuiConsole(duk_context* ctx)
{
	int state = duk_require_boolean(ctx, 0);
	if(state){
		log_gui_console = true;
		return 1;
	}
	log_gui_console = false;
	return 1;
}

///////////////////////////////////////////
int DKDuktapeJS::LowPowerMonitor(duk_context* ctx)
{
	if(!DKUtil::LowPowerMonitor()){ return 0; }
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::MessageBox(duk_context* ctx)
{
	DKString evt = duk_require_string(ctx, 0);
	DKString cmd = duk_require_string(ctx, 1);
	DKString message = duk_require_string(ctx, 2);
	DKClass::DKCreate("DKMessage.js");
	DKStringArray arry;
	toStringArray(arry, evt, ",");
	DKString type = arry[0];
	DKString id = arry[1];
	DKString value;
	if(arry.size() > 2){
		value = arry[2];
	}

	if(value.empty()){ DKEvent::SendEvent("DKMessage.html", cmd, id+","+type+","+message); return 0; }
	if(same(value,"CANCEL")){ return 0; }
	duk_push_string(ctx, value.c_str());
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::MouseToImage(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	int x;
	int y;
	if(!DKUtil::FindImageOnScreen(file, x, y)){ return 0; }
	if(!DKUtil::SetMousePos(x, y)){ return 0; }
	return 1;
}

//////////////////////////////////////////
int DKDuktapeJS::PhysicalMemory(duk_context* ctx)
{
	unsigned long long physicalMemory;
	if(!DKUtil::PhysicalMemory(physicalMemory)){ return 0; }
	duk_push_number(ctx, (double)physicalMemory);
	return 1;
}

//////////////////////////////////////////////
int DKDuktapeJS::PhysicalMemoryUsed(duk_context* ctx)
{
	unsigned long long physicalMemory;
	if(!DKUtil::PhysicalMemoryUsed(physicalMemory)){ return 0; }
	duk_push_number(ctx, (double)physicalMemory);
	return 1;
}

///////////////////////////////////////////////////
int DKDuktapeJS::PhysicalMemoryUsedByApp(duk_context* ctx)
{
	unsigned int physicalMemory;
	if(!DKUtil::PhysicalMemoryUsedByApp(physicalMemory)){ return 0; }
	duk_push_number(ctx, physicalMemory);
	return 1;
}

////////////////////////////////////
int DKDuktapeJS::PressKey(duk_context* ctx)
{
	int key = duk_require_int(ctx, 0);
	if(!DKUtil::PressKey(key)){
		return 0;
	}
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::QueueDuktape(duk_context* ctx)
{
	DKString code = duk_require_string(ctx, 0);
	DKDuktape::QueueDuktape(code);
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::ReleaseKey(duk_context* ctx)
{
	int key = duk_require_int(ctx, 0);
	if(!DKUtil::ReleaseKey(key)){
		return 0;
	}
	return 1;
}

//////////////////////////////////
int DKDuktapeJS::Reload(duk_context* ctx)
{
	DKDuktape::Reload();
	return 1;
}

//////////////////////////////////////
int DKDuktapeJS::RightClick(duk_context* ctx)
{
	if(!DKUtil::RightClick()){ return 0; }
	return 1;
}

///////////////////////////////
int DKDuktapeJS::Run(duk_context* ctx)
{
	DKString command = duk_require_string(ctx, 0);
	DKString params = duk_require_string(ctx, 1);
	if(!DKUtil::Run(command, params)){ return 0; }
	return 1;
}

/////////////////////////////////////////
int DKDuktapeJS::RunDuktape(duk_context* ctx)
{
	DKString code = duk_require_string(ctx, 0);
	DKString rval;
	DKDuktape::RunDuktape(code, rval);
	if(rval.empty()){
		return 0;
	}
	duk_push_string(ctx, rval.c_str());
	return 1;
}

/////////////////////////////////////////
int DKDuktapeJS::SetBrightness(duk_context* ctx)
{
	int percent = duk_require_int(ctx, 0);
	if(!DKUtil::SetBrightness(percent)){ return 0; }
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::SetClipboard(duk_context* ctx)
{
	DKString string = duk_require_string(ctx, 0);
	if(!DKUtil::SetClipboard(string)){ return 0; }
	return 1;
}

/////////////////////////////////////////////
int DKDuktapeJS::SetClipboardFiles(duk_context* ctx)
{
	DKString filelist = duk_require_string(ctx, 0);
	if(!DKUtil::SetClipboardFiles(filelist)){ return 0; }
	return 1;
}

/////////////////////////////////////////////
int DKDuktapeJS::SetClipboardImage(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	if(!DKUtil::SetClipboardImage(file)){ return 0; }
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::SetCursorPos(duk_context* ctx)
{
	int x = duk_require_int(ctx, 0);
	int y = duk_require_int(ctx, 1);
	if(!DKUtil::SetMousePos(x, y)){ return 0; }
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::SetFramerate(duk_context* ctx)
{
	int fps = duk_require_int(ctx, 0);
	DKUtil::SetFramerate(fps);
	return 1;
}

/////////////////////////////////////
int DKDuktapeJS::SetVolume(duk_context* ctx)
{
	int percent = duk_require_int(ctx, 0);
	if(!DKUtil::SetVolume(percent)){ return 0; }
	return 1;
}

///////////////////////////////////////
int DKDuktapeJS::ShowConsole(duk_context* ctx)
{
#ifdef WIN32
	HWND consoleWindow = GetConsoleWindow();
	ShowWindow(consoleWindow, SW_RESTORE);
#endif 
	return 1;
}

/////////////////////////////////
int DKDuktapeJS::Sleep(duk_context* ctx)
{
	int milliseconds = duk_require_int(ctx, 0);
	if(!DKUtil::Sleep(milliseconds)){ return 0; }
	return 1;
}

///////////////////////////////////////////
int DKDuktapeJS::StopPropagation(duk_context* ctx)
{
	return 0;
}

/////////////////////////////////////
int DKDuktapeJS::StrokeKey(duk_context* ctx)
{
	int key = duk_require_int(ctx, 0);
	if(!DKUtil::StrokeKey(key)){
		return 0;
	}
	return 1;
}

//////////////////////////////////
int DKDuktapeJS::System(duk_context* ctx)
{
	DKString command = duk_require_string(ctx, 0);
	if(!DKUtil::System(command)){ return 0; }
	return 1;
}

//////////////////////////////////////////
int DKDuktapeJS::TurnOffMonitor(duk_context* ctx)
{
	if(!DKUtil::TurnOffMonitor()){ return 0; }
	return 1;
}

/////////////////////////////////////////
int DKDuktapeJS::TurnOnMonitor(duk_context* ctx)
{
	if(!DKUtil::TurnOnMonitor()){ return 0; }
	return 1;
}

/////////////////////////////////
int DKDuktapeJS::Value(duk_context* ctx)
{
	DKString evt = duk_require_string(ctx, 0);
	DKString value = duk_require_string(ctx, 1);

	DKStringArray events;
	toStringArray(events, evt, ",");
	
	if(events.size() > 2){
		//DKINFO("Value:"+events[2]+"\n");
		if(!same(events[2],value)){ return 0; }
		return 1;
	}
	
	//DKINFO("Value:"+events[1]+"\n");
	DKERROR("DKDuktapeJS::Value(): We should not get here \n");
	if(!same(events[1],value)){ return 0; }
	return 1;

}

/////////////////////////////////////////
int DKDuktapeJS::VirtualMemory(duk_context* ctx)
{
	unsigned long long virtualMemory;
	if(!DKUtil::VirtualMemory(virtualMemory)){ return 0; }
	duk_push_number(ctx, (double)virtualMemory);
	return 1;
}

/////////////////////////////////////////////
int DKDuktapeJS::VirtualMemoryUsed(duk_context* ctx)
{
	unsigned long long virtualMemory;
	if(!DKUtil::VirtualMemoryUsed(virtualMemory)){ return 0; }
	duk_push_number(ctx, (double)virtualMemory);
	return 1;
}

//////////////////////////////////////////////////
int DKDuktapeJS::VirtualMemoryUsedByApp(duk_context* ctx)
{
	unsigned int virtualMemory;
	if(!DKUtil::VirtualMemoryUsedByApp(virtualMemory)){ return 0; }
	duk_push_number(ctx, virtualMemory);
	return 1;
}

////////////////////////////////////////
int DKDuktapeJS::WaitForImage(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	int timeout = duk_require_int(ctx, 1);
	if(!DKUtil::WaitForImage(file, timeout)){
		return 0;
	}
	duk_push_true(ctx);
	return 1;
}

#endif //USE_DKDuktape