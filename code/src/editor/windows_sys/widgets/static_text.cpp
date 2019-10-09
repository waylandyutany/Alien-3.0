#include "stdafx.h"

#if defined(AE_EDITOR)

#include "static_text.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_StaticText::C_StaticText(C_Widget* pParent, const char* sTitle) :
	C_Widget(pParent, windowType_StaticText, windowStyle_Visible | windowStyle_Child, 0)
{
	SetText(sTitle);
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)
