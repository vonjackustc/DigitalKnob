#ifdef USE_DKDuktape 
#include "DK/DKApp.h"
#include "DKRocket/DKDocument.h"
#include "DKRocket/DKRocket.h"


///////////////////////
bool DKDocument::Init()
{
	DKDEBUGFUNC();
	DKDuktape::AttachFunction("DKDocument_createElement", DKDocument::createElement);
	DKDuktape::AttachFunction("DKDocument_getElementById", DKDocument::getElementById);
	
	//DKClass::DKCreate("DKRocket/DKDocument.js");
	return true;
}

///////////////////////////////////////////////
int DKDocument::createElement(duk_context* ctx)
{
	DKDEBUGFUNC(ctx);
	DKString tag = duk_require_string(ctx, 0);
	Rocket::Core::Element* element = DKRocket::Get()->document->CreateElement(tag.c_str());
	if(!element){
		DKERROR("DKRocketJS::createElement(): element invalid\n");
		return false;
	}
	const void* elementAddress = static_cast<const void*>(element);
	std::stringstream ss;
	ss << elementAddress;  
	DKString str = ss.str(); 
	duk_push_string(ctx, str.c_str());
	return true;
}

////////////////////////////////////////////////
int DKDocument::getElementById(duk_context* ctx)
{
	DKDEBUGFUNC(ctx);
	DKString id = duk_require_string(ctx, 0);
	Rocket::Core::Element* element = DKRocket::Get()->document->GetElementById(id.c_str());
	if(!element){
		//DKERROR("DKRocketJS::getElementById(): element invalid\n");
		duk_push_null(ctx);
		return true;
	}

	const void* address = static_cast<const void*>(element);
	std::stringstream ss;
	ss << address;  
	DKString str = ss.str(); 

	duk_push_string(ctx, str.c_str());
	//duk_push_pointer(ctx, element);
	return true;
}

#endif //USE_DKDuktape