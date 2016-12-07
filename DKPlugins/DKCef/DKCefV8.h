//#ifdef USE_DKCef
#pragma once
#ifndef DKCefV8_H
#define DKCefV8_H

#include "DKCef.h"

/////////////////////////////////////////
class DKCefV8 : public DKObjectT<DKCefV8>
{
public:
	void Init();
	void End();

	static bool ClickImage(CefArgs args, CefReturn retval);
	static bool DKCreate_CPP(CefArgs args, CefReturn retval);
	static bool DKValid_CPP(CefArgs args, CefReturn retval);
	static bool Execute(CefArgs args, CefReturn retval);
	static bool Run(CefArgs args, CefReturn retval);
	static bool RunJavascript(CefArgs args, CefReturn retval);
};

REGISTER_OBJECT(DKCefV8, false);

#endif //DKCefV8_H
//#endif //USE_DKCef