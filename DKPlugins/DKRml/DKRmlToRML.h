#pragma once
#ifndef DKRmlToRML_H
#define DKRmlToRML_H

#include "DK/DK.h"
#include <RmlUi/Core.h>
#include <RmlUi/Controls.h>

///////////////////
class DKRmlToRML
{
public:
	DKString processed;
	bool HtmlToRml(const DKString& html, DKString& rml);
	bool Hyperlink(DKEvent* event);
	bool IndexToRml(const DKString& html, DKString& rml);
	bool PostProcess(Rml::Core::Element* element);
	static bool ResizeIframe(DKEvent* event);
	bool ClickIframe(DKEvent* event);
	bool MouseOverIframe(DKEvent* event);
	bool DKRmlToRML::Encode(std::string& data);
	bool TidyFile(const DKString& in, DKString& out);
};

#endif //DKRmlToRML_H