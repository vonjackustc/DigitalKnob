#include "stdafx.h"
#include "DKCefHandler.h"

////////////////////////////
DKCefHandler::DKCefHandler()
{
	DKClass::RegisterFunc("DKCefHandler::TestInt", &DKCefHandler::TestInt, this);
	DKClass::RegisterFunc("DKCefHandler::TestString", &DKCefHandler::TestString, this);
	DKClass::RegisterFunc("DKCefHandler::TestReturnInt", &DKCefHandler::TestReturnInt, this);
	DKClass::RegisterFunc("DKCefHandler::TestReturnString", &DKCefHandler::TestReturnString, this);

	DKClass::RegisterFunc("DKCefHandler::Minimize", &DKCefHandler::Minimize, this);
	DKClass::RegisterFunc("DKCefHandler::Restore", &DKCefHandler::Restore, this);
}

/////////////////////////////////////////////////////////
bool DKCefHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if(browser->IsPopup()){ return false; }
	DKApp::Exit();
	return false;
}

/////////////////////////////////////////////////////
bool DKCefHandler::TestInt(void* input, void* output)
{
	int in = *(int*)input;
	int out = in;
	*(int*)output = out;
	return true;
}

////////////////////////////////////////////////////////
bool DKCefHandler::TestString(void* input, void* output)
{
	std::string in = *(std::string*)input;
	std::string out = in;
	*(std::string*)output = out;
	return true;
}

///////////////////////////////////////////////////////////
bool DKCefHandler::TestReturnInt(void* input, void* output)
{
	int var = 1234;
	*(int*)output = var;
	return true;
}

//////////////////////////////////////////////////////////////
bool DKCefHandler::TestReturnString(void* input, void* output)
{
	std::string var = "Return test";
	*(std::string*)output = var;
	return true;
}


//////////////////////////////////////////////////////
bool DKCefHandler::Minimize(void* input, void* output)
{
	//TODO
	return false;
}

/////////////////////////////////////////////////////
bool DKCefHandler::Restore(void* input, void* output)
{
	//TODO
	return false;
}

