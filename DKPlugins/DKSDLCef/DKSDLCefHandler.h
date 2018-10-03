#pragma once
#ifndef DKSDLCefHandler_H
#define DKSDLCefHandler_H

#include <list>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_browser_process_handler.h>
#include <include/cef_display_handler.h>
#include "SDL.h"

#include "DK/DK.h"
#include "DK/DKApp.h"
#include "DKCef/DKCef.h"
#include "DKSDLCef/DKSDLCefKeyboard.h"
#include "DKSDLWindow/DKSDLWindow.h"
#include "DKSDLCef/DKSDLCef.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DKSDLCefHandler : public CefClient, public CefRenderHandler, public CefLoadHandler, public CefLifeSpanHandler, 
	public CefContextMenuHandler, public CefDownloadHandler, public CefDisplayHandler, 
	public CefDialogHandler
{
public:

	//static DKSDLCefHandler* GetInstance();

	DKSDLCefHandler(){}
	DKSDLWindow* dkSdlWindow;
	DKCef* dkCef;
	DKSDLCef* dkSdlCef;

	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler(){ return this; }
	virtual CefRefPtr<CefDialogHandler> GetDialogHandler(){ return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler(){ return this; }
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler(){ return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler(){ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler(){ return this; }
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler(){ return this; }

	/////////////////////////////////////////
	void DoFrame();
	//{ 
		//FIXME: this breaks SDL keyboard events for Mac OSX
	//	CefDoMessageLoopWork(); 
	//}

	//////////////////////////////////////////////////////////////
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		DKLog("DKSDLCefHandler::GetViewRect(CefBrowser, CefRect&)\n", DKDEBUG);

		rect = CefRect(0, 0, dkCef->width, dkCef->height);
		return true;
	}

	//////////////////////////////////////////////////
	void OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
	{
		DKLog("DKSDLCefHandler::OnBeforeContextMenu("+dkCef->id+")\n", DKDEBUG);

		model->Clear(); //remove original context menu

		DKString data;
		data += params->GetSelectionText();
		data += ",";
		data += params->GetSourceUrl();
		data += ",";
		data += params->GetLinkUrl();

		DKEvent::SendEvent("GLOBAL", "DKCef_ContextMenu", data);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
	{
		DKLog("DKSDLCefHandler::OnBeforeDownload("+suggested_name.ToString()+")\n", DKDEBUG);

		//DKLog("OriginalUrl: "+download_item->GetOriginalUrl().ToString()+")\n", DKDEBUG);
		UNREFERENCED_PARAMETER(browser);
		UNREFERENCED_PARAMETER(download_item);
		callback->Continue(suggested_name, true);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)
	{
		//TODO
		DKLog("DKSDLCefHandler::OnBeforePopup("+target_url.ToString()+","+target_frame_name.ToString()+","+toString(target_disposition)+")\n", DKDEBUG);

		return false;
		//DKEvent::SendEvent("GLOBAL", "DKCef_OnBeforePopup", target_url);
		if(target_disposition == WOD_NEW_FOREGROUND_TAB){
			dkCef->queue_new_browser = target_url;
		}
		else{
			dkCef->current_browser->GetMainFrame()->LoadURL(target_url.c_str());
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
	{
		//DKLog("DKSDLCefHandler::OnConsoleMessage()\n", DKINFO);

		CEF_REQUIRE_UI_THREAD();
		DKString msg = message.ToString();
		replace(msg, "%c", "");
		//DKLog("DKSDLCefHandler::OnConsoleMessage("+msg+","+source.ToString()+","+toString(line)+")\n", DKDEBUG);
		DKString string = message.ToString();
		replace(string,"%c","");
		int identifier = browser->GetIdentifier();
		DKLog("[CEF:"+toString(identifier)+"] "+string+"\n", DKINFO);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags)
	{
		CEF_REQUIRE_UI_THREAD();
		//DKLog("DKCefWindow::OnContextMenuCommand()\n", DKINFO);
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
	{
		//FIXME
		//DKLog("OnCursorChange()\n", DKINFO);
#ifdef WIN32
		HWND hwnd;
		if(!DKClass::CallFunc("DKSDLWindow::GetHandle", NULL, &hwnd)){ return; }
		if(!::IsWindow(hwnd)){ return; }
		SetClassLongPtr(hwnd, GCLP_HCURSOR, static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
		SetCursor(cursor);
#endif
#ifdef LINUX
		//Display* dpy;// = glfwGetX11Display();
		//Cursor c;
		//c = XCreateFontCursor(dpy, XC_xterm); 
		//XDefineCursor(dpy, w, c);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnFindResult(CefRefPtr<CefBrowser> browser, int identifier, int count, const CefRect& selectionRect, int activeMatchOrdinal, bool finalUpdate)
	{
		CEF_REQUIRE_UI_THREAD();
		DKLog("DKCefWindow::OnFindResult()\n", DKINFO);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnFileDialog(CefRefPtr<CefBrowser> browser, CefDialogHandler::FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)
	{
		DKLog("DKSDLCefHandler::OnFileDialog("+title.ToString()+","+default_file_path.ToString()+")\n", DKINFO);
		return false;
	}

	///////////////////////////////////////////////////////////////////////////
	void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
	{
		DKLog("DKSDLCefHandler::OnFullscreenModeChange()\n", DKDEBUG);

		if(fullscreen){
			dkCef->fullscreen = true;
			//SDL_SetWindowFullscreen(dkSdlWindow->sdlwin, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else{
			dkCef->fullscreen = false;
			//SDL_SetWindowFullscreen(dkSdlWindow->sdlwin, 0);
		}
		DKEvent::SendEvent("GLOBAL", "DKCef_OnFullscreen", toString(fullscreen));
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
	{
		DKLog("DKSDLCefHandler::OnLoadEnd("+toString(httpStatusCode)+")\n", DKDEBUG);
		if(frame->IsMain()){
			DKEvent::SendEvent("GLOBAL", "DKCef_OnLoadEnd", toString(httpStatusCode));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
	{ 
		DKLog("DKSDLCefHandler::OnLoadError("+toString(errorCode)+","+errorText.ToString()+","+failedUrl.ToString()+")\n", DKDEBUG);

		DKEvent::SendEvent("GLOBAL", "DKCef_OnLoadError", toString(errorCode));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
	{
		DKLog("DKSDLCefHandler::OnLoadingStateChange("+toString(isLoading)+","+toString(canGoBack)+","+toString(canGoForward)+")\n", DKDEBUG);

		for(unsigned int i=0; i<dkCef->browsers.size(); ++i){
			if(browser->GetIdentifier() == dkCef->browsers[i]->GetIdentifier()){
				DKEvent::SendEvent("GLOBAL", "DKCef_OnLoadingStateChange", toString(i));
				return;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
	{
		//DKLog("DKSDLCefHandler::OnPaint()\n", DKDEBUG);

		if(browser->GetIdentifier() != dkCef->current_browser->GetIdentifier()){ return; }
		if(type == PET_VIEW){
			if(dirtyRects.size() == 0){ return; }

			int w, h;
			SDL_QueryTexture(dkSdlCef->cef_image, NULL, NULL, &w, &h);
			if(w != width || h != height){
				dkSdlCef->cef_image = SDL_CreateTexture(dkSdlWindow->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
			}

			if (!dkSdlCef->background_image) {
				dkSdlCef->background_image = SDL_CreateTexture(dkSdlWindow->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
			}
			int w2, h2;
			SDL_QueryTexture(dkSdlCef->background_image, NULL, NULL, &w2, &h2);
			if (w2 != width || h2 != height) {
				dkSdlCef->background_image = SDL_CreateTexture(dkSdlWindow->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
			}

			unsigned char * texture_data = NULL;
			int texture_pitch = 0;
			if(SDL_LockTexture(dkSdlCef->background_image, NULL, (void **)&texture_data, &texture_pitch) == 0){
				//copies whole cef bitmap to sdl texture
				std::memcpy(texture_data, buffer, width * height * 4);
				SDL_UnlockTexture(dkSdlCef->background_image);
			}
		}

		else if(type == PET_POPUP){ //FIXME
			DKLog("DKSDLCefHandler::OnPaint(): type == PET_POPUP\n", DKINFO);
			//if(dirtyRects.size() == 0){ return; }
			if(!dkSdlCef->popup_image){
				dkSdlCef->popup_image = SDL_CreateTexture(dkSdlWindow->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
			}
			int w, h;
			SDL_QueryTexture(dkSdlCef->popup_image, NULL, NULL, &w, &h);
			if(w != width || h != height){
				dkSdlCef->popup_image = SDL_CreateTexture(dkSdlWindow->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
			}
			void* mPixels;
			int mPitch;
			if(SDL_LockTexture(dkSdlCef->popup_image, NULL, &mPixels, &mPitch) == 0){
				//copies popup bitmap to sdl texture
				std::memcpy(mPixels, buffer, width * height * 4);
				SDL_UnlockTexture(dkSdlCef->popup_image);
			}
		}

		if(dkSdlCef->cef_image) {	
			SDL_SetRenderTarget(dkSdlWindow->renderer, dkSdlCef->cef_image);
			SDL_RenderCopy(dkSdlWindow->renderer, dkSdlCef->background_image, NULL, NULL);
			if(dkSdlCef->popup_image){
				SDL_Rect popup;
				popup.x = dkSdlCef->popup_rect.x;
				popup.y = dkSdlCef->popup_rect.y;
				popup.w = dkSdlCef->popup_rect.width;
				popup.h = dkSdlCef->popup_rect.height;
				SDL_RenderCopy(dkSdlWindow->renderer, dkSdlCef->popup_image, NULL, &popup);	
			}
			SDL_SetRenderTarget(dkSdlWindow->renderer, NULL);
		}
	}

	//////////////////////////////////////////////////////////
	void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
	{
		//TODO
		DKLog("DKSDLCefHandler::OnPopupShow()\n", DKDEBUG);
		return;

		if(!show){
			dkSdlCef->popup_image = NULL;
			return;
		}
	}

	////////////////////////////////////////////////////////////////////
	void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
	{
		//TODO
		DKLog("DKSDLCefHandler::OnPopupSize()\n", DKDEBUG);
		return;

		if(rect.width <= 0 || rect.height <= 0){ return; }
		dkSdlCef->popup_rect = rect;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
	{
		CEF_REQUIRE_UI_THREAD();
		//DKLog("OnPreKeyEvent(): char="+toString(event.character)+", native="+toString(event.native_key_code)+", mods="+toString(event.modifiers)+"\n", DKINFO);

		if(event.type == KEYEVENT_RAWKEYDOWN){
			//DKLog("OnPreKeyEvent(): RawKeyDown: "+toString(event.character)+"\n", DKINFO);
			//#ifdef WIN32
			DKEvent::SendEvent("GLOBAL", "keydown", toString(event.windows_key_code));
			//#else
			//			DKEvent::SendEvent("GLOBAL", "keydown", toString(event.character));
			//#endif
		}
		if(event.type == KEYEVENT_KEYDOWN){
			DKLog("OnPreKeyEvent(): KeyDown: "+toString(event.character)+"\n", DKINFO);
		}
		if(event.type == KEYEVENT_KEYUP){
			//DKLog("OnPreKeyEvent(): KeyUp: "+toString(event.character)+"\n", DKINFO);
		}
		if(event.type == KEYEVENT_CHAR){
			//DKLog("OnPreKeyEvent(): KeyChar: "+toString(event.character)+"\n", DKINFO);
			DKEvent::SendEvent("GLOBAL", "keypress", toString(event.character));
		}

		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnPrintDialog(CefRefPtr<CefBrowser> browser, bool has_selection, CefRefPtr<CefPrintDialogCallback> callback)
	{
		DKLog("OnPrintDialog()\n", DKINFO);
		return true;
	}

	bool OnPrintJob(CefRefPtr<CefBrowser> browser, const CefString& document_name, const CefString& pdf_file_path, CefRefPtr<CefPrintJobCallback> callback){ return true; }
	void OnPrintReset(CefRefPtr<CefBrowser> browser){}
	void OnPrintSettings(CefRefPtr<CefBrowser> browser, CefRefPtr<CefPrintSettings> settings, bool get_defaults){}
	void OnPrintStart(CefRefPtr<CefBrowser> browser){}

	//////////////////////////////////////////////////////
	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) 
	{
		//DKLog("DKSDLCefHandler::OnProcessMessageReceived()\n", DKINFO);

		if(message->GetName() == "GetFunctions"){
			//DKLog("DKSDLCefHandler::OnProcessMessageReceived(GetFunctions)\n", DKINFO);
			DKV8::GetFunctions(browser);
		}

		if(has(message->GetName(),"CallFunc(")){
			//DKLog("DKSDLCefHandler::OnProcessMessageReceived(CallFunc)\n", DKINFO);

			//get function name
			DKString func = message->GetName();
			replace(func,"CallFunc(", "");
			replace(func,")", "");

			//get arguments
			CefRefPtr<CefListValue> arguments = message->GetArgumentList();
			DKV8::Execute(browser, func, arguments);
		}

		return false;
	}

	/*
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser, const CefString& requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback)
	{
	CEF_REQUIRE_UI_THREAD();
	DKLog("DKCefWindow::OnRequestGeolocationPermission()\n", DKINFO);

	callback->Continue(true);
	return true;
	}
	*/

	//////////////////////////////////////////////////////////////
	bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text)
	{
		//FIXME: this is never called
		CEF_REQUIRE_UI_THREAD();
		DKLog("DKCefWindow::OnTooltip()\n", DKINFO);
		return true;
	}

	IMPLEMENT_REFCOUNTING(DKSDLCefHandler);
};

#endif //DKSDLCefHandler_H