#include "DK/stdafx.h"
#ifdef WIN32
#include <tchar.h>
#include <Commctrl.h>
#include "DK/DKFile.h"
#include "DKAssets/DKAssets.h"
#include "DKWindow/DKWindow.h"
#include "DKHandles/DKHandles.h"

DKStringArray DKHandles::_windows;
std::vector<HWND> DKHandles::handle;
bool DKHandles::searching = false;
WNDPROC DKHandles::prevWndProc;
HHOOK DKHandles::hMouseHook;

//////////////////////
bool DKHandles::Init()
{
	DKClass::DKCreate("DKHandlesJS");
	DKClass::DKCreate("DKHandlesV8");
	highlight = false;
	currentHandle = 0;
	return true;
}

/////////////////////
bool DKHandles::End()
{
	return true;
}



///////////////////////
bool DKHandles::Click()
{
	if(handle.empty()){ return false; }
	SendMessage(handle[currentHandle], BM_CLICK, 0, 0);
	return true;
}

/////////////////////////////
bool DKHandles::DoHighlight()
{
	if(!highlight){ return false; }
	if(handle.empty()){ return false; }

	RedrawWindow(GetDesktopWindow(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW); //FIXME

	RECT rect;
	GetWindowRect(handle[currentHandle], &rect);
	HDC screenDC = ::GetDC(GetDesktopWindow());

	HPEN pen, oldPen;
	pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); 
	oldPen = (HPEN)SelectObject(screenDC, pen);

	::MoveToEx(screenDC, rect.left, rect.top, NULL);
	::LineTo(screenDC, rect.right, rect.top);
	::LineTo(screenDC, rect.right, rect.bottom);
	::LineTo(screenDC, rect.left, rect.bottom);
	::LineTo(screenDC, rect.left, rect.top);
	::ReleaseDC(0, screenDC);

	SelectObject(screenDC, oldPen); 
	DeleteObject(pen);
	return true;
}

////////////////////////////////////////
bool DKHandles::GetClass(DKString& clas)
{
	char classname[256];
	if(handle.empty()){ return false; }
	if(!GetClassName(handle[currentHandle], classname, 256)){
		DKLog("DKHandles::GetClass("+clas+")\n", DKWARN);
		return false; 
	}
	clas = classname;
	return true;
}

////////////////////////////
bool DKHandles::GetHandles()
{
	if(handle.empty() || handle[0] == NULL){ //Window is not open so don't look for its items.
		DKLog("DKHandles::GetHandles() window not found! \n", DKERROR);
		return false;
	}

	//Window IS open, so look for all of the items and assign handles
	bool searchFromBase = true;
	unsigned int s = 0;
	unsigned int h = 1;
	while(s < handle.size()){
		HWND temp = NULL;
		if(searchFromBase){
			temp = FindWindowEx(handle[s], NULL, NULL, NULL);
			if(temp){ handle.push_back(temp); }
			searchFromBase = false;
		}
		else{
			temp = FindWindowEx(handle[s], handle[h-1], NULL, NULL);
			if(temp){ handle.push_back(temp); }
		}
		if(temp != NULL){
			h++;
		}
		else{
			s++; searchFromBase = true;
		}
	}

	return true;
}

//////////////////////////////////
bool DKHandles::GetLeft(int& left)
{
	if(handle.empty()){ return false; }
	RECT rect;
	GetWindowRect(handle[currentHandle], &rect);
	left = rect.left;
	return true;
}

///////////////////////////////////////////
bool DKHandles::GetParent(DKString& parent)
{
	if(handle.empty()){ return false; }
	HWND par = ::GetParent(handle[currentHandle]);
	if(!par){ return false; }
	int len = SendMessage(par, WM_GETTEXTLENGTH, 0, 0);
    char* buffer = new char[len];
    SendMessage(par, WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
    parent = buffer;
	return true;
}

/////////////////////////////////////////
bool DKHandles::GetString(DKString& text)
{
	if(handle.empty()){ return false; }
	int len = SendMessage(handle[currentHandle], WM_GETTEXTLENGTH, 0, 0);
	char* buffer = new char[len];
	SendMessage(handle[currentHandle], WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
	text = buffer;
	return true;
}

////////////////////////////////
bool DKHandles::GetTop(int& top)
{
	if(handle.empty()){ return false; }
	RECT rect;
	GetWindowRect(handle[currentHandle], &rect);
	top = rect.top;
	return true;
}

//////////////////////////////////////////////////
bool DKHandles::GetWindows(DKStringArray& windows)
{
	_windows.clear();
	bool rval = (EnumWindows(GetWindows, NULL) != 0);
	windows = _windows;
	return rval;
}

////////////////////////////
bool DKHandles::NextHandle()
{
	if(handle.empty()){
		DKLog("DKHandles::NextHandle(): handle is empty\n", DKWARN);
		return false; 
	}
	if(currentHandle < handle.size()-1){currentHandle++;}
	if(currentHandle <= handle.size()){
		DoHighlight();
	}
	return true;
}

////////////////////////////
bool DKHandles::PrevHandle()
{
	if(handle.empty()){
		DKLog("DKHandles::PrevHandle(): handle is empty\n", DKWARN);
		return false; 
	}
	if(currentHandle > 0){currentHandle--;}
	if(currentHandle <= handle.size()){
		DoHighlight();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool DKHandles::SendHook(const DKString& window, const DKString& handle, const DKString& data)
{
	DKLog("DKHandles::SendHook("+window+", "+handle+", "+data+")\n", DKDEBUG);

	if(!SetWindowHandle(window, 1)){ return false; }
	if(!SetHandle(toInt(handle), 1)){ return false; }

	DKStringArray arry;
	toStringArray(arry, data, ",");

	if(same(arry[0], "click")){
		Click();
	}
	if(same(arry[0], "GetValue")){
		DKString value;
		GetString(value);
		DKLog("Hook GetValue returned: "+value+"\n", DKINFO);
	}
	if(same(arry[0], "SetValue")){
		SetString(arry[1]);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool DKHandles::SetHandle(const DKString& clas, const DKString& value, unsigned int timeout)
{
	unsigned int t = 0;
	unsigned int h = 0;
	DKString text;
	char classname[256];

	while(t < timeout){
		GetHandles();
		for(h=0; h<handle.size(); h++){
			int len = SendMessage(handle[h], WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendMessage(handle[h], WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
			DKString text = buffer;
			if (value.empty()) {
				int test = 0;
			}
			if(text == value){
				if(!GetClassName(handle[h], classname, 256)){
					DKLog("DKHandles::SetHandle("+clas+","+value+"): GetClassName failed. \n", DKWARN);
					return false; 
				}
				if(clas == (DKString)classname){
					currentHandle = h;
					return true;
				}
			}
		}
		Sleep(1000); //FIXME
		++t;
	}
	
	DKLog("DKHandles::SetHandle("+clas+","+value+","+toString(timeout)+"): timed out.\n", DKWARN);
	return false;
}

//////////////////////////////////////////////////////////////////////
bool DKHandles::SetHandle(const DKString& value, unsigned int timeout)
{
	unsigned int t = 0;
	unsigned int h = 0;
	DKString text;
	while(t < timeout){
		GetHandles();
		for(h=0; h<handle.size(); h++){
			int len = SendMessage(handle[h], WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendMessage(handle[h], WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
			text = buffer;
			if(text == value){
				currentHandle = h;
				return true;
			}
		}
		Sleep(1000); //FIXME
		++t;
	}

	DKLog("DKHandles::SetHandle("+value+","+toString(timeout)+"): timed out.\n", DKWARN);
	return false;
}

///////////////////////////////////////////////////////////////////
bool DKHandles::SetHandle(unsigned int index, unsigned int timeout)
{
	unsigned int t = 0;
	while(index > handle.size() && t < timeout){
		GetHandles();
		Sleep(1000); //FIXME
		++t;
	}
	if(index > handle.size()){
		DKLog("DKHandles::SetHandle("+toString(index)+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	currentHandle = index;
	return true;
}

///////////////////////////////////////////////
bool DKHandles::SetString(const DKString& text)
{
	if(handle.empty()){ return false; }
	SendMessage(handle[currentHandle], WM_SETTEXT, (WPARAM)text.size(), (LPARAM)text.c_str());
	return true;
}

////////////////////////////////////////////////////////////////////////////
bool DKHandles::SetWindowHandle(const DKString& title, unsigned int timeout)
{
	handle.clear();
	HWNDname temp;
	temp.caption = title.c_str();
	unsigned int t = 0;
	while(EnumWindows(FindWindow, (LPARAM)&temp) && t < timeout){
		Sleep(1000); //FIXME
		++t;
	}
	if(t >= timeout){
		DKLog("DKHandles::SetWindowHandle("+title+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	
	GetHandles();
	SetHandle(0,1);
	DKLog("Selected Window: "+title+"\n", DKINFO);
	return true;
}

/////////////////////////////////////////////
bool DKHandles::ShowWindow(unsigned int flag)
{
	::ShowWindow(handle[currentHandle], flag);
	return true;
}

/////////////////////////////
bool DKHandles::StartSearch()
{
	//FIXME
	//DKLog("DKHandles::StartSearch()\n", DKINFO);

	//HWND hwnd = ::GetActiveWindow();
	HWND hwnd = NULL;
	DKWindow::GetHandle((void*&)hwnd);
	if(!hwnd){
		DKLog("DKHandles::StartSearch(): hwnd is NULL\n", DKINFO);
		return false;
	}
	
	//TEST - are we getting the main DK window, set the title of the window? 
	SetWindowText(hwnd, "This is a test"); //Set the title to test

	//SetWindowSubclass(hwnd, &SearchProc, 1, 0); //FIXME - NOT WORKING
	//prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)&SearchProc);
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &SearchProc, DKWindows::hInstance, NULL);

	searching = TRUE;
	//TODO - MoveCursorPositionToBullsEye
	//TODO - Set the screen cursor to the BullsEye cursor.

	SetCapture(hwnd);

	//TODO - Hide main window.
	return true;
}

/////////////////////////////////
bool DKHandles::ToggleHighlight()
{
	if(highlight){
		highlight = false;
		DoHighlight();
		return true;
	}
	highlight = true;
	DoHighlight();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
bool DKHandles::WaitForHandle(const DKString& clas, const DKString& value, int timeout)
{
	int i = 0;
	DKString text;
	char classname[256];
	
	while(text != value && clas != (DKString)classname && i < timeout){
		GetHandles();
		for(unsigned int i=0; i<handle.size(); i++){
			int len = SendMessage(handle[i], WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendMessage(handle[i], WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
			DKString text = buffer;
			if(text == value){
				if(!GetClassName(handle[i], classname, 256)){
					DKLog("DKHandles::SetHandle("+clas+","+value+"): GetClassName failed. \n", DKWARN);
					return false; 
				}
			}
		}
		Sleep(1000); //FIXME
		++i;
	}
	if(i >= timeout){
		DKLog("DKHandles::WaitForHandle("+clas+","+value+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////
bool DKHandles::WaitForHandle(const DKString& value, int timeout)
{
	int i = 0;
	DKString text;
	while(text != value && i < timeout){
		GetHandles();
		for(unsigned int i=0; i<handle.size(); i++){
			int len = SendMessage(handle[i], WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendMessage(handle[i], WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);
			text = buffer;
		}
		Sleep(1000); //FIXME
		++i;
	}
	if(i >= timeout){
		DKLog("DKHandles::WaitForHandle("+value+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////
bool DKHandles::WaitForHandle(unsigned int index, int timeout)
{
	int i = 0;
	while(index > handle.size() && i < timeout){
		GetHandles();
		Sleep(1000); //FIXME
		++i;
	}
	if(i >= timeout){
		DKLog("DKHandles::WaitForHandle("+toString(index)+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	//currentHandle = index;
	return true;
}

/////////////////////////////////////////////////////////////////
bool DKHandles::WaitForWindow(const DKString& title, int timeout)
{
	//FIXME - this is blocking,  thread this out
	int i = 0;
	while(!WindowExists(title) && i < timeout){
		Sleep(1000); //FIXME
		++i;
	}
	if(i >= timeout){
		DKLog("DKHandles::WaitForWindow("+title+","+toString(timeout)+"): timed out.\n", DKWARN);
		return false;
	}
	return true;
}

///////////////////////////////////////////////////
bool DKHandles::WindowExists(const DKString& title)
{
	HWNDname temp;
	temp.caption = title.c_str();
	if(EnumWindows(FindWindow, (LPARAM)&temp)){
		//TODO: GetLastError
		return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////
BOOL CALLBACK DKHandles::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char class_name[80];
	char title[80];
	GetClassName(hwnd,class_name, sizeof(class_name));
	GetWindowText(hwnd,title,sizeof(title));

	/*
	if(!same("",title)){
	DKLog("Window: ");
	DKLog(title);
	DKLog(" Class: ");
	DKLog(class_name);
	DKLog("\n");
	}
	*/

	return TRUE;
}

/////////////////////////////////////////////////////////////
BOOL CALLBACK DKHandles::FindWindow(HWND hwnd, LPARAM lparam) 
{     
	HWNDname *temp = (HWNDname*)lparam;
	static TCHAR buffer[50];      
	GetWindowText(hwnd, buffer, 50);     
	if(strcmp(buffer, temp->caption) == 0){ 
		handle.push_back(hwnd);
		return FALSE;     
	}      
	return TRUE; 
}

////////////////////////////////////////////////////////////////////
BOOL CALLBACK DKHandles::FindWindowPartial(HWND hwnd, LPARAM lparam) 
{     
	HWNDname *temp = (HWNDname*)lparam;
	static TCHAR buffer[50];      
	GetWindowText(hwnd, buffer, 50);     
	if(_tcsstr(buffer, temp->caption)) { 
		handle.push_back(hwnd);
		return FALSE;     
	}      
	return TRUE; 
}

/////////////////////////////////////////////////////////////
BOOL CALLBACK DKHandles::GetWindows(HWND hwnd, LPARAM lparam) 
{     
	static TCHAR buffer[50];      
	GetWindowText(hwnd, buffer, 50);
	DKString title = buffer;
	if(!title.empty()){
		_windows.push_back(title);
	}    
	return true; 
}

//////////////////////////////////////////////////////////////////////////////////////////
//LRESULT CALLBACK DKHandles::SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
//LRESULT CALLBACK DKHandles::SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
LRESULT CALLBACK DKHandles::SearchProc(int code, WPARAM wParam, LPARAM lParam)
{
	//FIXME - NOT WORKING
	//DKLog("DKHandles::SearchProc\n", DKINFO);

	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if(pMouseStruct != NULL){
		if(wParam == WM_LBUTTONDOWN){
			printf("clicked"); 
		}
		printf("Mouse position X = %d  Mouse Position Y = %d\n", pMouseStruct->pt.x,pMouseStruct->pt.y);
	}

	/*
	switch(uMsg){
		case WM_MOUSEMOVE :
		{
			DKLog("DKHandles::SearchProc(): WM_MOUSEMOVE\n", DKINFO);
			if(searching){
				// Only when we have started the Window Searching operation will we track mouse movement.
				//DoMouseMove(hwndDlg, uMsg, wParam, lParam);
			}
			break;
		}
		case WM_LBUTTONUP :
		{
			DKLog("DKHandles::SearchProc(): WM_LBUTTONUP\n", DKINFO);
			if(searching){
				// Only when we have started the window searching operation will we
				// be interested when the user lifts up the left mouse button.
				//DoMouseUp(hwndDlg, uMsg, wParam, lParam);
				searching = false;
			}
			break;
		}
		default :
		{
			DKLog("DKHandles::SearchProc(): uMsg = "+toString(uMsg)+"\n", DKINFO);
			break;
		}
	}
	*/

	//return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	//return CallWindowProc(prevWndProc, hwnd, uMsg, wParam, lParam);
	return CallNextHookEx(hMouseHook, code, wParam, lParam);
}


#endif //WIN32