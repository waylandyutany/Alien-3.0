#include "stdafx.h"

#if defined(AE_EDITOR)

#include "buttons.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_Button::C_Button(C_Widget* pParent, const char* sTitle, T_WindowStyle style, T_ControlID id) :
	C_Widget(pParent, windowType_Button, style, id)
{
	SetText(sTitle);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_PushButton::C_PushButton(C_Widget* pParent, const char* sTitle, T_ControlID id) :
	C_Button(pParent, sTitle, windowStyle_Visible | windowStyle_TabStop | windowStyle_Child | windowStyle_PushButton, id)
{
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
