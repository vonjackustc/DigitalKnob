#include "DK/stdafx.h"

#include "DK/DKApp.h"
#include "DK/DKOsInfo.h"
#include "DK/DKFile.h"

int DKApp::argc;
char** DKApp::argv;
bool DKApp::active = false;
bool DKApp::loaded = false;
bool DKApp::paused = false;
std::vector<boost::function<void()> > DKApp::loop_funcs;

UINT32 DKApp::lastSecond;

UINT32 DKApp::now;
UINT32 DKApp::lastFrame; 
int DKApp::_fps = 60;
int DKApp::ticksPerFrame = 1000 / DKApp::_fps;

//#ifdef USE_Boost_System
#ifdef WIN32
HINSTANCE DKApp::hInstance = 0L;
int main(int argc, char **argv);
//////////// WIN32 MAIN //////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    DKApp::hInstance = hInstance;
    return main(__argc, __argv);
}
#endif

/////////// MAIN ////////////////
#ifndef ANDROID
int main(int argc, char **argv)
{
#if __cplusplus <= 199711L
	DKLog("C++98 \n", DKINFO);
#else
	DKLog("C++11 \n", DKINFO);
#endif

	DKApp::argc = argc;
    DKApp::argv = argv;

	DKUtil::SetMainThreadNow();
	
#ifndef IOS
	DKFile::appfilename = argv[0];

#ifdef WIN32
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)DKApp::ConsoleHandler, true)){
		DKLog("Could not set Console Handler. \n", DKERROR);
	}
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 640, 1024, SWP_NOSIZE | SWP_NOZORDER);

	/////  Set the window title
	DKString title; 
	DKFile::GetExeName(title);
	title += " - WIN32";
#ifdef DEBUG
	title += " DEBUG ";
#else
	title += " RELEASE ";
#endif
	DKString mTime;
	DKString file;
	DKFile::GetFullExeName(file);
	DKFile::GetModifiedTime(file, mTime);
	title += mTime;
	SetConsoleTitle(title.c_str());
#endif

	DKString info;
	GetOSInfo(info);
	DKLog(info+"\n", DKINFO);

	DKString date;
	DKUtil::GetDate(date);
	DKString time;
	DKUtil::GetTime(time);
	DKLog(date+" "+time+"\n", DKINFO);
	
	//print args
	if(DKApp::argc > 1){
		for(int i = 1; i < DKApp::argc; ++i){
			DKLog("argv["+toString(i)+"] = "+toString(DKApp::argv[i])+"\n", DKINFO);
	    }
    }
	
	DKCreate("DKAssets");
	DKCreate("DKDuktape");
	DKCreate("DKDebug");
	DKApp dkapp;
	DKObject* app = DKCreate("App"); //App.h/App.cpp (user code)
	dkapp.Init();
	dkapp.Loop();
	dkapp.Exit();

#else
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    UIApplicationMain(argc, argv, nil, @"iphoneViewerAppDelegate");
    [pool release];
    return 0;
#endif
}
#endif //!ANDROID
//#endif //USE_Boost_System
//////////////////////////////////


//////////////
DKApp::DKApp()
{
	DKLog("DKApp::DKApp()\n", DKDEBUG);
#ifdef ANDROID
	DKUtil::mainThreadId = (int)pthread_self();
#endif
}

//////////////////
void DKApp::Init()
{
	DKLog("DKApp::Init()\n", DKDEBUG);
	active = true;
	DKUtil::GetTicks(now);
	DKUtil::GetTicks(lastFrame);
	DKUtil::GetTicks(lastSecond);
}

//////////////////
void DKApp::Loop()
{
	DKLog("DKApp::Loop()\n", DKDEBUG);
	while(active){
		DoFrame();
	}
}

/////////////////////
void DKApp::DoFrame()
{
	if(paused){ return; }

	//Framerate / cpu limiter
	DKUtil::GetTicks(now);
	int delta = now - lastFrame;
	if(delta < ticksPerFrame){
		UINT32 sleep = ticksPerFrame - delta;
		DKUtil::Sleep(sleep);
	}
	DKUtil::GetTicks(lastFrame);

	for(unsigned int i = 0; i < loop_funcs.size(); ++i){
		if (active){
			loop_funcs[i](); //Call loop functions
		}
	}

	if (((now - lastSecond) / 1000) > 1.0){ // 1 second
		SendEvent("GLOBAL", "second", ""); //This might be to heavy
		lastSecond = now;
	}
}

//////////////////
void DKApp::Exit()
{
	DKLog("DKApp::Exit(): \n", DKINFO);
	active = false;

#ifdef ANDROID
	CallJavaFunction("Exit","");
#endif

#ifdef WIN32
	if(GetCurrentThreadId() != DKUtil::mainThreadId){   //GetCurrentThreadId not available for android
		DKLog("DKApp::Exit(): attempting to call Exit() from another thread \n", DKWARN);
	}
#endif	

	DKClass::CloseAll();
	exit(0);
}

/////////////////////////
int DKApp::GetFramerate()
{
	return _fps;
}

/////////////////////////////////
void DKApp::SetFramerate(int fps)
{
	DKLog("DKApp::SetFramerate("+DKString(toString(fps))+")\n", DKINFO);
	_fps = fps;
	if(_fps == 0){ ticksPerFrame = 0; return; }
	ticksPerFrame = 1000 / _fps;
}

#ifdef WIN32
/////////////////////////////////////////////
bool WINAPI DKApp::ConsoleHandler(DWORD type)
{
	//DKLog("DKApp::ConsoleHandler(DWORD)\n", DKDEBUG);
	switch(type){
		case CTRL_CLOSE_EVENT:
			DKApp::Exit();
			return true;
	}
	return false;
}
#endif
