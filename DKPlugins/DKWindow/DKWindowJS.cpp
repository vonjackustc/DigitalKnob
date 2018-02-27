#ifdef USE_DKDuktape 
#include "DKWindowJS.h"
#include "DKWindow.h"


///////////////////////
bool DKWindowJS::Init()
{
	DKLog("DKWindowJS::Init()\n", DKDEBUG);

	DKDuktape::AttachFunction("DKWindow_TestInt", DKWindowJS::TestInt);
	DKDuktape::AttachFunction("DKWindow_TestString", DKWindowJS::TestString);
	DKDuktape::AttachFunction("DKWindow_TestReturnInt", DKWindowJS::TestReturnInt);
	DKDuktape::AttachFunction("DKWindow_TestReturnString", DKWindowJS::TestReturnString);

	DKDuktape::AttachFunction("DKWindow_Create", DKWindowJS::Create);
	DKDuktape::AttachFunction("DKWindow_Fullscreen", DKWindowJS::Fullscreen);
	DKDuktape::AttachFunction("DKWindow_GetHeight", DKWindowJS::GetHeight);
	DKDuktape::AttachFunction("DKWindow_GetMouseX", DKWindowJS::GetMouseX);
	DKDuktape::AttachFunction("DKWindow_GetMouseY", DKWindowJS::GetMouseY);
	DKDuktape::AttachFunction("DKWindow_GetPixelRatio", DKWindowJS::GetPixelRatio);
	DKDuktape::AttachFunction("DKWindow_GetWidth", DKWindowJS::GetWidth);
	DKDuktape::AttachFunction("DKWindow_GetX", DKWindowJS::GetX);
	DKDuktape::AttachFunction("DKWindow_GetY", DKWindowJS::GetY);
	DKDuktape::AttachFunction("DKWindow_Hide", DKWindowJS::Hide);
	DKDuktape::AttachFunction("DKWindow_IsFullscreen", DKWindowJS::IsFullscreen);
	DKDuktape::AttachFunction("DKWindow_IsVisible", DKWindowJS::IsVisible);
	DKDuktape::AttachFunction("DKWindow_MessageBox", DKWindowJS::MessageBox);
	DKDuktape::AttachFunction("DKWindow_Minimize", DKWindowJS::Minimize);
	DKDuktape::AttachFunction("DKWindow_Restore", DKWindowJS::Restore);
	DKDuktape::AttachFunction("DKWindow_SetHeight", DKWindowJS::SetHeight);
	DKDuktape::AttachFunction("DKWindow_SetIcon", DKWindowJS::SetIcon);
	DKDuktape::AttachFunction("DKWindow_SetTitle", DKWindowJS::SetTitle);
	DKDuktape::AttachFunction("DKWindow_SetWidth", DKWindowJS::SetWidth);
	DKDuktape::AttachFunction("DKWindow_SetX", DKWindowJS::SetX);
	DKDuktape::AttachFunction("DKWindow_SetY", DKWindowJS::SetY);
	DKDuktape::AttachFunction("DKWindow_Show", DKWindowJS::Show);
	DKDuktape::AttachFunction("DKWindow_Windowed", DKWindowJS::Windowed);
	return true;
}


/////////////////////////////////////////
int DKWindowJS::TestInt(duk_context* ctx)
{
	int input = duk_require_int(ctx, 0);
	int output = DKWindow::TestInt(input);
	duk_push_int(ctx, output);
	return 1;
}

////////////////////////////////////////////
int DKWindowJS::TestString(duk_context* ctx)
{
	DKString input = duk_require_string(ctx, 0);
	DKString output = DKWindow::TestString(input);
	duk_push_string(ctx, output.c_str());
	return 1;
}

///////////////////////////////////////////////
int DKWindowJS::TestReturnInt(duk_context* ctx)
{
	int rval = DKWindow::TestReturnInt();
	duk_push_int(ctx, rval);
	return 1;
}

//////////////////////////////////////////////////
int DKWindowJS::TestReturnString(duk_context* ctx)
{
	DKString rval = DKWindow::TestReturnString();
	duk_push_string(ctx, rval.c_str());
	return 1;
}



////////////////////////////////////////
int DKWindowJS::Create(duk_context* ctx)
{
	if(!DKWindow::Create()){ return 0; }
	return 1;
}

/////////////////////////////////////////
int DKWindowJS::SetIcon(duk_context* ctx)
{
	DKString file = duk_require_string(ctx, 0);
	if (!DKWindow::SetIcon(file)) { return 0; }
	return 1;
}

//////////////////////////////////////////
int DKWindowJS::SetTitle(duk_context* ctx)
{
	DKString string = duk_require_string(ctx, 0);
	if(!DKWindow::SetTitle(string)){ return 0; }
	return 1;
}

//////////////////////////////////////
int DKWindowJS::GetX(duk_context* ctx)
{
	duk_push_int(ctx, DKWindow::GetX());
	return 1;
}

//////////////////////////////////////
int DKWindowJS::GetY(duk_context* ctx)
{
	duk_push_int(ctx, DKWindow::GetY());
	return 1;
}

//////////////////////////////////////////
int DKWindowJS::GetWidth(duk_context* ctx)
{
	duk_push_int(ctx, DKWindow::GetWidth());
	return 1;
}

///////////////////////////////////////////
int DKWindowJS::GetHeight(duk_context* ctx)
{
	int height;
	if(!DKWindow::GetHeight(height)){ return 0; }
	duk_push_int(ctx, height);
	return 1;
}

//////////////////////////////////////
int DKWindowJS::SetX(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	DKWindow::SetX(toInt(data));
	return 1;
}

//////////////////////////////////////
int DKWindowJS::SetY(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	DKWindow::SetY(toInt(data));
	return 1;
}

//////////////////////////////////////////
int DKWindowJS::SetWidth(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	DKWindow::SetWidth(toInt(data));
	return 1;
}

///////////////////////////////////////////
int DKWindowJS::SetHeight(duk_context* ctx)
{
	DKString data = duk_require_string(ctx, 0);
	DKWindow::SetHeight(toInt(data));
	return 1;
}

///////////////////////////////////////////////
int DKWindowJS::GetPixelRatio(duk_context* ctx)
{
	duk_push_number(ctx, DKWindow::GetPixelRatio());
	return 1;
}

//////////////////////////////////////////////
int DKWindowJS::IsFullscreen(duk_context* ctx)
{
	bool isFullscreen = DKWindow::IsFullscreen();
	if(!isFullscreen){ 
		return 0;
	}
	duk_push_int(ctx, isFullscreen);
	return 1;
}

////////////////////////////////////////////
int DKWindowJS::Fullscreen(duk_context* ctx)
{
	DKWindow::Fullscreen();
	return 1;
}

//////////////////////////////////////////
int DKWindowJS::Windowed(duk_context* ctx)
{
	DKWindow::Windowed();
	return 1;
}

//////////////////////////////////////////
int DKWindowJS::Minimize(duk_context* ctx)
{
	DKWindow::Minimize();
	return 1;
}

/////////////////////////////////////////
int DKWindowJS::Restore(duk_context* ctx)
{
	DKWindow::Restore();
	return 1;
}

///////////////////////////////////////////
int DKWindowJS::IsVisible(duk_context* ctx)
{
	bool isVisible = DKWindow::IsVisible(); 
	if(!isVisible){ 
		return 0;
	}
	duk_push_int(ctx, isVisible);
	return 1;
}

//////////////////////////////////////
int DKWindowJS::Hide(duk_context* ctx)
{
	DKWindow::Hide();
	return 1;
}

//////////////////////////////////////
int DKWindowJS::Show(duk_context* ctx)
{
	DKWindow::Show();
	return 1;
}

///////////////////////////////////////////
int DKWindowJS::GetMouseX(duk_context* ctx)
{
	int mouseX = DKWindow::GetMouseX();
	duk_push_int(ctx, mouseX);
	return 1;
}

///////////////////////////////////////////
int DKWindowJS::GetMouseY(duk_context* ctx)
{
	int mouseY = DKWindow::GetMouseY();
	duk_push_int(ctx, mouseY);
	return 1;
}

////////////////////////////////////////////
int DKWindowJS::MessageBox(duk_context* ctx)
{
	DKString msg = duk_require_string(ctx, 0);
	if(!DKClass::CallFunc("DKWindow::MessageBox", &msg, NULL)){ return 0; }
	return 1;
}

#endif //USE_DKDuktape