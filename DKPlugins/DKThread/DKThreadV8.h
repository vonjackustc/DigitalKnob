#ifdef USE_DKCef
#pragma once
#ifndef DKThreadV8_H
#define DKThreadV8_H

#include "DKCef.h"

///////////////////////////////////////////////
class DKThreadV8 : public DKObjectT<DKThreadV8>
{
public:
	void Init();
	void End();
	
	static bool MyFunc(CefArgs args, CefReturn retval);
	
	static bool GetThreadNames(CefArgs args, CefReturn retval);
	static bool _DKQueue(CefArgs args, CefReturn retval);
	static void QueueItem();
};

REGISTER_OBJECT(DKThreadV8, true);

#endif //DKThreadV8_H
#endif //USE_DKCef