#pragma once
#if !defined(AE_STATIC_TEXT_H) && defined(AE_EDITOR)
#define AE_STATIC_TEXT_H

#include "widget.h"
#include "../base/window_constants.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_StaticText : public C_Widget
	{
		C_StaticText();
		NOCOPY_CLASS(C_StaticText);
	public:
		C_StaticText(C_Widget* pParent, const char* sTitle);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_STATIC_TEXT_H) && defined(AE_EDITOR)
