#ifdef USE_DKDuktape 
#include "DKOcrJS.h"
#include "DKOcr.h"


////////////////////
bool DKOcrJS::Init()
{
	DKDebug();
	DKDuktape::AttachFunction("DKOcr_ImageToText", DKOcrJS::ImageToText);
	return true;
}

//////////////////////////////////////////
int DKOcrJS::ImageToText(duk_context* ctx)
{
	DKDebug(ctx);
	DKString file = duk_require_string(ctx, 0);
	DKString text;
	if(!DKOcr::ImageToText(file, text)){ return 0; }
	duk_push_string(ctx, text.c_str());
	return 1;
}

#endif //USE_DKDuktape