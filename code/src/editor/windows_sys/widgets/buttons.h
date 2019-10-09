#pragma once
#if !defined(AE_BUTTONS_H) && defined(AE_EDITOR)
#define AE_BUTTONS_H

#include "widget.h"
#include "../base/window_constants.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_Button : public C_Widget
	{
		NOCOPY_CLASS(C_Button);
		C_Button();
	protected:
		C_Button(C_Widget* pParent, const char* sTitle, T_WindowStyle, T_ControlID);
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_PushButton : public C_Button
	{
		C_PushButton();
		NOCOPY_CLASS(C_PushButton);
	public:
		C_PushButton(C_Widget* pParent, const char* sTitle, T_ControlID);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_BUTTONS_H) && defined(AE_EDITOR)
