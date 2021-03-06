#pragma once
#ifndef DKRocketToRML_H
#define DKRocketToRML_H

#include "DK/DK.h"
#include <Rocket/Core/Core.h>
#include <Rocket/Controls/Controls.h>
#include <Rocket/Core/ElementUtilities.h>

///////////////////
class DKRocketToRML
{
public:
	DKString processed;
	bool HtmlToRml(const DKString& html, DKString& rml);
	bool Hyperlink(DKEvent* event);
	bool IndexToRml(const DKString& html, DKString& rml);
	bool PostProcess(Rocket::Core::Element* element);
	static bool ResizeIframe(DKEvent* event);
	bool ClickIframe(DKEvent* event);
	bool MouseOverIframe(DKEvent* event);
	bool DKRocketToRML::Encode(std::string& data);
	bool TidyFile(const DKString& in, DKString& out);
};

#endif //DKRocketToRML_H